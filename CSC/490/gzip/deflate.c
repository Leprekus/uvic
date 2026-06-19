#include "deflate.h"
#include "huffman.h"
#include "lzss.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// type definitions and state
typedef struct __attribute__((__packed__)) {
	u8  OPTIONS; 
	u16 LEN;
	u16 NLEN;
} Block0;
struct DeflateCtx {
	int FD;
	u8 *stream;
	size_t len;
	pHandler pre;
	pHandler emit;
	BitVec *v;
	
	// STATE
	size_t written;
};
typedef enum { DISTANCE, LENGTH } MatchTokenKind;
typedef enum  { REV, NOREV } Packing;
typedef struct {
	u16 code, code_size, 
	    offset, offset_size, 
	    range_start, range_end, value;
	MatchTokenKind kind;
} DeflateMatchToken;
typedef struct {
	struct { DeflateMatchToken len, dist; };
} Pair;
DeflateCtx * deflate_global_ctx= NULL;

/* distance[i] = { code, num_offset_bits, range_start, range_end }*/
 static u16 distance_codes [30][4] = {
            {0,0,1,1},         {1,0,2,2},          {2,0,3,3},           {3,0,4,4},           {4,1,5,6},
            {5,1,7,8},         {6,2,9,12},         {7,2,13,16},         {8,3,17,24},         {9,3,25,32},
            {10,4,33,48},      {11,4,49,64},       {12,5,65,96},        {13,5,97,128},       {14,6,129,192},
            {15,6,193,256},    {16,7,257,384},     {17,7,385,512},      {18,8,513,768},      {19,8,769,1024},
            {20,9,1025,1536},  {21,9,1537,2048},   {22,10,2049,3072},   {23,10,3073,4096},   {24,11,4097,6144},
            {25,11,6145,8192}, {26,12,8193,12288}, {27,12,12289,16384}, {28,13,16385,24576}, {29,13,24577,32768},
 };
static u16 length_codes[29][4] =  {
            {257,0,3,3},     {258,0,4,4},     {259,0,5,5},     {260,0,6,6},     {261,0,7,7},
            {262,0,8,8},     {263,0,9,9},     {264,0,10,10},   {265,1,11,12},   {266,1,13,14},
            {267,1,15,16},   {268,1,17,18},   {269,2,19,22},   {270,2,23,26},   {271,2,27,30},
            {272,2,31,34},   {273,3,35,42},   {274,3,43,50},   {275,3,51,58},   {276,3,59,66},
            {277,4,67,82},   {278,4,83,98},   {279,4,99,114},  {280,4,115,130}, {281,5,131,162}, 
            {282,5,163,194}, {283,5,195,226}, {284,5,227,257}, {285,0,258,258}
};

//FUNCTION PROTOTYPES
/* 
 * we process len bytes of the stream, 
 * lookahead - len get processed in deflate_close()
 * to emit a final header 
 * */
size_t block0(u8 *stream, size_t len, pHandler emit, DeflateStatus status);
size_t block1(u8 *stream, size_t len, BitVec *v, pHandler emit, DeflateStatus);
size_t block2(u8 *stream, size_t len, DeflateStatus status);
BITVEC_STATUS push_bits(BitVec *v, u64, size_t n, pHandler emit);
BITVEC_STATUS push_literal(BitVec *v, Code c, pHandler emit);
BITVEC_STATUS push_eob(BitVec *v, pHandler emit);
DeflateMatchToken get_token(u16 v, MatchTokenKind m);
Pair create_backref(u16 len, u16 dist, Packing p);
Code create_literal(u16 literal, Packing p);

Code create_literal(u16 literal, Packing p) {
	if(0 <= literal && literal <= 143){ 
		// Seems to work
		// 0b0011_0000 through 0b1011_1111
		literal = 0x0030 + literal;
		assert(0x30 <= literal && literal <= 0xBF);
		literal = (p == REV) ? reverse_u8(literal) : literal;
		return (Code){ .code = literal, .size = 8 };
		//return push_bits(v, reverse_u8(literal), 8, emit);
		
	}

	if(144 <= literal && literal <= 255) {
		// seems to work
		//0b110010000 through 0b111111111
		literal = 0x0190 + (literal - 144);
		//printf("literal(%#3hx) reversed(%#3hx)", literal, reverse_u16(literal));
		assert(0x0190 <= literal && literal <= 0x01FF);
		literal = (p==REV) ? reverse_u16(literal)>>7 : literal;
		return (Code){ .code = literal, .size = 9 };
	}
	fprintf(stderr, "create_literal failed");
	return (Code){};
}
/* wrapper for bit pushing to handle retries if buffer is full,
* - State is NOT OUT_OF_BOUNDS: Returns the vector status as-is immediately.
* - State IS OUT_OF_BOUNDS: Flushes data via 'emit', clears the vector, and
 * forwards immediate status after a push retry*/
BITVEC_STATUS push_bits(BitVec *v, u64 bits, size_t n, pHandler emit) {
	BITVEC_STATUS s = bit_vec_push_nbits(v, bits, n);
	// Seems to be working
	//TODO: fix chopping before clearing
	// case 1: vector is full we clear and retry
	// case 2: vector's  capacity is less than size_t n.
	// then we chop input and push, otherwise our bitpacking loses the
	// current index and gets shifted causing incorrect output.  
	if(s == OUT_OF_BOUNDS){
		size_t remaining = bit_vec_remaining_bits(v);
		size_t min = (remaining < n) ? remaining : n;
		while(n) {
			if(!remaining) {
				emit(bit_vec_data(v), bit_vec_byte_count(v));
				bit_vec_clear(v);
			} else {
				s = bit_vec_push_nbits(v, bits, min);
				if(s != SUCCESS) return s;
				if(bit_vec_is_full(v)) {
					emit(bit_vec_data(v), bit_vec_byte_count(v));
					bit_vec_clear(v);
				}
			}
			bits >>= min; 
			n -= min;
			remaining = bit_vec_remaining_bits(v);
			min = (remaining < n) ? remaining : n;
		}
		return s;
		
	}
	return s;
} 
BITVEC_STATUS push_literal(BitVec *v, Code c, pHandler emit) {
	
	return push_bits(v, c.code, c.size, emit);
};
BITVEC_STATUS push_eob(BitVec *v, pHandler emit) {
	return push_bits(v, 0x00, 7, emit);
}
// TODO: if smth fails check hdist & hlength
u16 hdist(u16 d) {
	u32 i = 0;
	u32 nbits = distance_codes[i][1];
	u32 range = (1 << nbits);
 	while(range < d) {
		nbits = distance_codes[++i][1];
		range += (1 << nbits);
	};
	return i;
}

u16 hlength(u16 l) {
	u32 i = 0;
	u32 nbits = length_codes[i][1];
	// first code holds length 3 = 2^0 + 2
	// where 0 is the number of offset bits for the ith code
	u32 range = (1 << nbits) + 2; 

	while(range < l) {
		nbits = length_codes[++i][1];
		range += (1 << nbits);
	};

	return i;
}


DeflateMatchToken get_token(u16 v, MatchTokenKind m) {
	u16 i;
	u16 *code; 
	switch(m) {
		case LENGTH:
			i = hlength(v);
			code = length_codes[i];
			break;
		case DISTANCE:
			i = hdist(v);
			code = distance_codes[i];
			break;
	}
	u16 offset_size = code[1],
	rs = code[2], // range start
	re = code[3], // range end
	offset = v - rs;
	// get code length in bits
	u16 n = 0, temp = code[0];	
	while(temp > 0) { n++; temp = temp >> 1; }
	
	//printf("%s(%#hx, %#hx) offset(%hu, %hu)", (m==LENGTH)?"len":"dist",code[0], n, offset, offset_size);
	return (DeflateMatchToken){
		.code = code[0], 
		.code_size = n,
		.offset = offset,
		.offset_size = offset_size, 
		.range_start = rs, 
		.range_end = re, 
		.value = v,
		.kind = m

	};
}

Pair create_backref(u16 len, u16 dist, Packing p) {
	DeflateMatchToken tok_len = get_token(len, LENGTH);	
	DeflateMatchToken tok_dist = get_token(dist, DISTANCE);	
	pHandler emit = deflate_global_ctx->emit;
	

	u16 len_size = 0;
	// PROCESS LENGTH
	if(256 <= tok_len.code && tok_len.code <= 279) {
		//0b0000000 through 0b0010111
		tok_len.code = 
			(p==REV)?
			reverse_u16(0x00 + (tok_len.code - 256)) >> (16-7) :
			0x00 + (tok_len.code - 256);
		tok_len.code_size = 7;

	} else if(280 <= tok_len.code && tok_len.code <= 287) {
		// 0b11000000 through 0b11000111
		tok_len.code = 
			(p==REV) ?
			reverse_u16(0xC0 + (tok_len.code - 280)) >> (16-8):
			0xC0 + (tok_len.code - 280);
		tok_len.code_size = 8;

	} else 
	fprintf(stderr, "expected len between [257, 279] got len(%hu)", len);
	// PROCESS DISTANCE
	tok_dist.code = (p==REV) ? reverse_u16(tok_dist.code) >> (16-5):
		tok_dist.code;
	return (Pair){
		.len = tok_len,
		.dist = tok_dist
	};

}

void push_backref(BitVec *v, Pair p, pHandler emit) {
	DeflateMatchToken tok_len = p.len;
	DeflateMatchToken tok_dist = p.dist;
	push_bits(v, tok_len.code, tok_len.code_size, emit);
	push_bits(v, tok_len.offset, tok_len.offset_size, emit);

	push_bits(v, tok_dist.code, 5, emit);
	push_bits(v, tok_dist.offset, tok_dist.offset_size, emit);
	//printf("<(%#hx, %#hx, %#hx, %#hx), (%#hx, %#hx, %#hx, %#hx)>", tok_len.code, tok_len.code_size, tok_len.offset, tok_len.offset_size, 
//			tok_dist.code, tok_dist.code_size, tok_dist.offset, tok_dist.offset_size);

}



DeflateCtx *deflate_ctx_init(
	int FD, u8 *stream, size_t len, pHandler pre, pHandler emit) {
	if(!stream || len < 2 || !emit) {
		fprintf(stderr, "Error invalid value: stream(%p), len(%zu), or emit(%p)", (void *)stream, len, (void *)emit);
		return NULL;
	}

	DeflateCtx *ctx = malloc(sizeof(DeflateCtx));
	if(!ctx) return NULL;
	BitVec *v = bit_vec_init(1024<<3);
	if(!v) return NULL;
	ctx->v = v;
	ctx->FD = FD;
	ctx->stream = stream;
	ctx->len = len;
	ctx->pre = pre;
	ctx->emit = emit;
	ctx->written = 0;
	deflate_global_ctx = ctx;
	return ctx;
}
#define buffer_is_full(n, s, e)((n) == ((e)-(s)))
#define buffer_has_bytes(n)((n) > 0)
ssize_t deflate_read(DeflateCtx *ctx) {
	if(!ctx) {
		fprintf(stderr, "Error: context is null\n");
		return -1;
	}
	ssize_t 
		s1 = 0, e1 = ctx->len / 2,
		s2 = ctx->len / 2, e2 = ctx->len;

	size_t n1 = read(ctx->FD, ctx->stream + s1, e1 - s1),
		n2 = 0;
	if(n1 < 0) return n1;
	/* 1. read into first half of buffer:
	 * 	case 1: second half full and stream continues
	 * 	case 2: second half full and stream ends
	 * 	case 3: second not full and stream ends
	 * 2. read into second half of buffer: 
	 *	NOTE: swap s1 <-> s2, e1 <-> e2 
	 * 	(s1, e1 now point to upper half of buffer)
	 *
	 * 	case 1: 
	 * 		a. load second half, 
	 * 		b. deflate first half, 
	 * 		c. swap pointers second half <-> first half, repeat a.
	 *
	 * 	case 2:
	 * 		from case 1 do: a., b., c.
	 * 		d. buffer_has_bytes = false and we flush
	 * 		outside the while loop emitting a BFINAL = 1
	 *
	 * 	case 3: same as case 2.
	 * 	*/
	while( /*buffer_is_full(n1, s1, e1)*/ buffer_has_bytes(n1) ) {
		//printf("state: s1(%zu), e1(%zu), s2(%zu), e2(%zu), n1(%zd), n2(%zu)", s1, e1, s2, e2, n1, n2);
		if(
		    buffer_is_full(
			    (n2 = read(ctx->FD, ctx->stream + s2, e2 - s2)), s2, e2)
		   ) {
			//printf("buffer_full s1(%zu), e1(%zu), s2(%zu), e2(%zu), n1(%zd), n2(%zd)", s1, e1, s2, e2, n1, n2);
			ctx->pre(ctx->stream + s1, n1);
			ctx->written += 
				deflate(ctx->stream + s1, n1, ctx->v, ctx->emit, RUN);	
			
			s1 ^= s2; e1 ^= e2; n1 = n2;
			s2 ^= s1; e2 ^= e1; n2 = 0;
			s1 ^= s2; e1 ^= e2;

		} else if(n2 < 0) return n2;
		else if(buffer_has_bytes(n2)){
			//printf("buffer_has_bytes s1(%zu), e1(%zu), s2(%zu), e2(%zu), n1(%zd), n2(%zd)", s1, e1, s2, e2, n1, n2);
			ctx->pre(ctx->stream + s1, n1);
			ctx->written += 
				deflate(ctx->stream  + s1, n1, ctx->v, ctx->emit, RUN);
			ctx->pre(ctx->stream + s2, n2);
			ctx->written += 
				deflate(ctx->stream  + s2, n2, ctx->v, ctx->emit, FINISH);
			break;
		}
		/* if buffer 2 is empty only deflate buf 1 */
		else {

			//printf("buffer_is_empty s1(%zu), e1(%zu), s2(%zu), e2(%zu), n1(%zd), n2(%zd)", s1, e1, s2, e2, n1, n2);
			ctx->pre(ctx->stream + s1, n1);
			ctx->written +=
				deflate(ctx->stream + s1, n1, ctx->v, ctx->emit, FINISH);
			break;
		} 
		

	}
	return ctx->written;
}
/*
 * Entry point, so stream should always start at 0.
 * idea: calculate entropy as a heuristic to choose block type
 * https://en.wikipedia.org/wiki/Entropy_(information_theory)
 * H(X) := sum(p(x)) * ln(p(x))/ln(2)
 * */
/* deflate(bitstream) -> decompressed | huffman | huffman + lzss */
size_t deflate(u8 *stream, size_t read, BitVec *v, pHandler emit, DeflateStatus status) {
	
	return block2(stream, read, status);
	return block1(stream, read, v, emit, status);
	return block0(stream, read, emit, status);
	
}

void deflate_close(size_t written, pHandler emit) {
	// when an empty file is given: output just the header
	// ccurs when we create an empty gzip
	if(written == 0) {
		block0(NULL, 0, emit, FINISH);
		return;
	}
}


size_t block0(u8 *stream, size_t len, pHandler emit, DeflateStatus status) {
	Block0 b = {
		.OPTIONS = status == FINISH,
		.LEN = len,
		.NLEN = ~len
	};
	/*u8 b_stream[sizeof(b)];
	memcpy(b_stream, &b, sizeof(b));*/

	emit(&b, sizeof(Block0));
	emit(stream, len);
	return len;
}
// defines whether bitstring should be reversed
// TODO: refactor so functions use deflate_global ctx
// NOTE IF CALLER DOES NOT IMMEDIATELY COPY THIS VALUE
// IT MAY LEAD TO UNDEFINED BEHAVIOR
void bit_stream_handler(LzssData d) {
	DeflateCtx *ctx = deflate_global_ctx;
	switch(d.kind) {
		case LITERAL:
			//deflate_global_ctx->emit(&d.literal, 1);
			push_literal(
				ctx->v, 
				create_literal(d.literal, REV), 
				ctx->emit);
			break;
		case BACKREF:
			push_backref(
				deflate_global_ctx->v,
				create_backref(
					d.ref.length, d.ref.distance, REV),
				ctx->emit
			);
			break;
		default:
			fprintf(stderr, "Error: LzssData.kind is neither LITERAL or BACKREF");
	}
}
size_t block1(u8 *stream, size_t len, BitVec *v, pHandler emit, DeflateStatus status) {
	u64 header = (0x02) | (status == FINISH); // BFINAL=1 BTYPE=01
	push_bits(v, header, 3, emit);
	//for(size_t i = 0; i < len; i++) {
	//	for(size_t j = i + 3; j <= len; j++) {

	//	}
	//	push_literal(v, stream[i], emit);
	//}
	lzss_compress_stream(stream, len, bit_stream_handler);
	
	
	push_eob(v, emit); // push eob to BitVec
	if(status == FINISH) {
		bit_vec_pad(v); // pad BitVec if it's last block
		if(bit_vec_bit_count(v)) 
			emit(bit_vec_data(v), bit_vec_byte_count(v));
	}
	return len;
}
//u16 distance_map[30] = {0};
//u16 ll_map[285] = {0};
#define CL_HIST_SIZE 19
typedef struct {
		u16 length;
		u8 repeat;
        u8 repeat_len;
		u8 c_op; // code operation
} CLSymbol;/* count LL and Distance frequencies */

typedef struct {
    u32 ll_hist[512];
    u16 ll_max;
    u32 dist_hist[512];
    u16 dist_max;
    // histogram for cl codes
    u32 cl_hist[CL_HIST_SIZE];
    u8 cl_max;
    u8 cl_code_lengths[CL_HIST_SIZE];
    u16 cl_codes[CL_HIST_SIZE];
} Block2Ctx;
/* initialized in each call to block2 so context is always fresh */
Block2Ctx b2ctx;
// The RFC 1951 permutation order for code length codes
static const uint8_t CL_ORDER[19] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

CLSymbol cls_create(u16 l, u8 r, u8 c) {
    assert(0 <= l && l <= 15);
    b2ctx.cl_hist[l]++;
    size_t rl;
    switch(c) {
        case 16: rl = 2; break;
        case 17: rl = 3; break;
        case 18: rl = 7; break;
        default: rl = 0; break;
    }
    u8 max = (rl > c) ? rl : c; 
    b2ctx.cl_max = (b2ctx.cl_max > max) ? b2ctx.cl_max : max;
    assert(b2ctx.cl_max < 19);
    return (CLSymbol) {
        .length = l,
        .repeat = r,
        .repeat_len = rl,
        .c_op   = c
    };
}
void block2_stream_handler(LzssData d){
	DeflateCtx *ctx = deflate_global_ctx;
	if(d.kind == LITERAL) {
		Code c = create_literal(d.literal, NOREV);
		assert(0 <= c.code && c.code < 512);
		b2ctx.ll_hist[c.code]++;
        b2ctx.ll_max = (c.code > b2ctx.ll_max) ? c.code : b2ctx.ll_max;
	} else if(d.kind == BACKREF) {
		Pair p = create_backref(d.ref.length, d.ref.distance, NOREV);
		assert(0 <= p.len.code && p.len.code < 512);
		assert(0 <= p.dist.code && p.dist.code < 512);
		b2ctx.ll_hist[p.len.code]++;
        b2ctx.ll_max = (p.len.code > b2ctx.ll_max) ? p.len.code : b2ctx.ll_max;

		b2ctx.dist_hist[p.dist.code]++;
        b2ctx.dist_max = (p.dist.code > b2ctx.dist_max) ? p.dist.code : b2ctx.dist_max;

	} else fprintf(stderr, "Error: LzssData.kind is neither LITERAL or BACKREF");
}
// IR of LL & DIST encoded table 
CLSymbol cl_buf[512]; 
u16 cl_buf_idx = 0;
void rle_run(u8 code_lengths[], u16 max_code) {
    u32 prev =  code_lengths[0];
    u32 curr = -1;
    size_t i = 0;
    u8 run = 1;
    size_t written = 0;

    // used to determine which symbol to emit
    bool is_zero   = prev == 0;	
    bool is_length = prev > 0;
    for(i = 1; i < max_code + 1 ; i++){
        curr = code_lengths[i];

        bool changed_val = prev != curr;
        bool match = prev == curr;
        is_zero   = prev == 0;
        is_length = prev > 0;

        // curr = i - 1, prev = i - 2
        if(match) {
            assert(i > 0); // ensure we can index curr and prev
            if(is_length && run == 6) {
                cl_buf[cl_buf_idx++] = cls_create(code_lengths[i], run - 3, 16);
                written += run;
                run = 0;
                //printf("len-match run(%hu) at (%zu)\n", run, i);
            }
            if(is_zero && run == 138) {
                cl_buf[cl_buf_idx++] = cls_create(code_lengths[i], run - 11, 18);
                written += run;
                //printf("zero-match run(%hu) at (%zu)\n", run, i);
                run = 0;
            }
            // in a match [a, a, a] we flush the first three
            // and set run = 1 overcounting the last element
            // so we must offset the count by 1
            run ++;
        } else {
            if(is_length) {
                if(run > 2) {
                    cl_buf[cl_buf_idx++] = cls_create(code_lengths[i], run - 3, 16);
                } else {
                    cl_buf[cl_buf_idx++] = cls_create(code_lengths[i - 1], 0, 0);
                    cl_buf[cl_buf_idx++] = cls_create(code_lengths[i], 0, 0);
                }
                //printf("len-match-break run(%hu) at (%zu)\n", run, i);

            }
            if(is_zero) {
                if(run >= 11) {
                    cls_create(code_lengths[i], run - 11, 18);
                } else if(run >= 3) {
                    cls_create(code_lengths[i], run - 3, 17);
                } else {
                    cls_create(code_lengths[i - 1], 0, 0);
                    cls_create(code_lengths[i], 0, 0);
                }
                //printf("zero-match-break run(%hu) at (%zu)\n", run, i);

            }

            written += run;
            run = 1;
        }
        prev = curr;
        assert(cl_buf_idx <= 316);
        assert((changed_val && !match) ||
               (!changed_val && match));
        assert((is_zero && !is_length) || 
               (!is_zero && is_length));


    }
    assert(i == max_code + 1);
    if(run){
        if(is_length) {
            if(run > 2) {
                cl_buf[cl_buf_idx++] = cls_create(code_lengths[i], run - 3, 16);
            } else {
                cl_buf[cl_buf_idx++] = cls_create(code_lengths[i - 1], 0, 0);
                cl_buf[cl_buf_idx++] = cls_create(code_lengths[i], 0, 0);
            }
            //printf("len-remainder run(%hu) at (%zu)\n", run, i);

        }
        if(is_zero) {
            if(run >= 11) {
                cls_create(code_lengths[i], run - 11, 18);
            } else if(run >= 3) {
                cls_create(code_lengths[i], run - 3, 17);
            } else {
                cls_create(code_lengths[i - 1], 0, 0);
                cls_create(code_lengths[i], 0, 0);
            }

            //printf("zero-remainder run(%hu) at (%zu\n", run, i);
        }
        written += run - 1;
    }

	assert(written == max_code);
    assert(cl_buf_idx < 512);
}

size_t block2(u8 *stream, size_t len, DeflateStatus status) {
	DeflateCtx *ctx = deflate_global_ctx;
    b2ctx = (Block2Ctx) {
        .ll_hist = {0},
        .ll_max = 0,
        .dist_hist = {0},
        .dist_max = 0,
        .cl_hist = {0},
        .cl_max = 0,
        .cl_code_lengths = {0},
        .cl_codes = {0}
    };
	
							     //
	//push_bits(ctx->v, 0x00, 1024, ctx->emit);
	// emit literals and backreferences
	lzss_compress_stream(stream, len, block2_stream_handler); 
	// at this point all the literals and backreferences have been emitted,
	// frequencies have been counted, and they are stored in stream
    u8 ll_code_lengths[b2ctx.ll_max + 1]; 
    u16 ll_codes[b2ctx.ll_max + 1]; 
    compute_canon_hf_codes(15, b2ctx.ll_max, b2ctx.ll_hist, ll_code_lengths, ll_codes);
    rle_run(ll_code_lengths, b2ctx.ll_max);

    u8 dist_code_lengths[b2ctx.ll_max + 1]; 
    u16 dist_codes[b2ctx.ll_max + 1]; 
    if(!b2ctx.dist_max) {
        b2ctx.dist_max = 1;
        dist_code_lengths[0] = 1;
        dist_code_lengths[1] = 1;
        b2ctx.dist_hist[0] = 1;
        b2ctx.dist_hist[1] = 1;
    } else 
        compute_canon_hf_codes(5, b2ctx.dist_max, b2ctx.dist_hist, dist_code_lengths, dist_codes);
    rle_run(dist_code_lengths, b2ctx.dist_max);
	
    compute_canon_hf_codes(5, b2ctx.cl_max, b2ctx.cl_hist, b2ctx.cl_code_lengths, b2ctx.cl_codes);
	//printf("written(%zu), len(%d) run(%hu)\n", written, ll_dist_max, run);
    printf("LL CODES\n");
    print_table(b2ctx.ll_hist, ll_codes, ll_code_lengths, b2ctx.ll_max); 
    printf("\n");

    printf("DIST CODES\n");
    print_table(b2ctx.dist_hist, dist_codes, dist_code_lengths, b2ctx.dist_max); 
    printf("\n");

    printf("CL CODES\n");
    print_table(b2ctx.cl_hist, b2ctx.cl_codes, b2ctx.cl_code_lengths, b2ctx.cl_max); 
    printf("\n");
    //encode cl_buf using the cl prefix codes


    // HEADER
    assert(28 <= b2ctx.ll_max && b2ctx.ll_max <= 285);
    assert(1 <= b2ctx.dist_max && b2ctx.dist_max <= 29);
	push_bits(ctx->v, status == FINISH, 1, ctx->emit); // is_last
	push_bits(ctx->v, 0x2, 2, ctx->emit); // btype 
    // Rule # 1 applies to hlit, hdist, hclen, & CL len table
    // hlit - number of LL entries
	push_bits(ctx->v, b2ctx.ll_max - 257, 5, ctx->emit);  assert(b2ctx.ll_max - 257 <= 31);
    // hdist - number of Dist entries
	push_bits(ctx->v, b2ctx.dist_max - 1, 5, ctx->emit); 
    // hclen - number of CL entries
    size_t cl_table_len = 19;
    while(cl_table_len > 4 && b2ctx.cl_code_lengths[CL_ORDER[cl_table_len - 1]] == 0) {
        cl_table_len--;
    }
	push_bits(ctx->v, cl_table_len - 4, 5, ctx->emit); 
    // CL len table 
    for(size_t i = 0; i < cl_table_len; i++) {
        size_t idx = CL_ORDER[i];
        u8 len = b2ctx.cl_code_lengths[idx];
        assert(len <= 7);                   
        push_bits(ctx->v, len, 3, ctx->emit);
    }
        
    // LL & Dist len table
    for(size_t i = 0; i < cl_buf_idx; i++) {
        CLSymbol sym = cl_buf[i];
        u16 len = sym.length;
        size_t size = 0;
        while(len > 0) { len >>= 1; size++; }
	    push_bits(ctx->v, sym.length, size, ctx->emit);
        if(sym.c_op) {
	        push_bits(ctx->v, b2ctx.cl_codes[sym.c_op], size, ctx->emit);
	        push_bits(ctx->v, sym.repeat, sym.repeat_len, ctx->emit);
        }
    }
	//push_bits(ctx->v, reverse_u8(15 - 4), 5, ctx->emit); // LL_Dist_lens - encoded tables
	// after building the tree, we retrieve the huffman codes
	// to compute the CL symbols
	// TODO:
	// 1. add test cases for HLIT & HDIST when there are few symbols, 
	// such as in aaabaaa.txt
	
	if(status == FINISH) {
		bit_vec_pad(ctx->v); // pad BitVec if it's last block
		if(bit_vec_bit_count(ctx->v)) 
			ctx->emit(bit_vec_data(ctx->v), bit_vec_byte_count(ctx->v));
	}
	/*
	 * STEPS:
	 * 1. count LL & distance frequencies
	 * 2. generate huffman code from the alphabet union(LL, distance)
	 * 3. construct the bitstream using huffman codes
	 * 4. encode CL, LL, and Dist tables
	 *
	 * Notes:
	 * - CL code lengths have a special order
	 * - CL encodings must be at most 7 bits (so the length is encoded in 3 bits) 
	 *
	 * ENCODING BLOCK HEADER
	 * - tell HLIT how many symbols are encoded, 
	 *   n = 0 up to max symbol used from LL codes so HLIT = n - 257
	 *  - Repeat for HDIST
	 *  - 
	 * */
	return len;
}
