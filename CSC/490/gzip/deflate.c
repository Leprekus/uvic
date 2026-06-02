#include "deflate.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// type definitions and state
#define false 0
#define true 1
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
typedef u8 bool;

// parsing
BITVEC_STATUS push_literal(BitVec *v, u16 literal) {
	if(0 <= literal && literal <= 143){ 
		// Seems to work
		// 0b0011_0000 through 0b1011_1111
		literal = 0x0030 + literal;
		assert(0x30 <= literal && literal <= 0xBF);
		return bit_vec_push_nbits(v, reverse_u8(literal), 8);
		
	}

	if(144 <= literal && literal <= 255) {
		// seems to work
		//0b110010000 through 0b111111111
		literal = 0x0190 + (literal - 144);
		//printf("literal(%#3hx) reversed(%#3hx)", literal, reverse_u16(literal));
		assert(0x0190 <= literal && literal <= 0x01FF);
		return bit_vec_push_nbits(v, reverse_u16(literal)>>7, 9);
	}

	fprintf(stderr, "expected literal between [0, 143] or [144, 255] got literal(%hu)", literal);
	return -1;
};
void push_eob(BitVec *v) {
	bit_vec_push_nbits(v, 0x00, 7);
}
void push_backref(BitVec *v, u16 len, u16 dist) {
	if(257 <= len && len   <= 279 &&
	   0   <= dist && dist <= 29
	   ) { //0b0000001 through 0b0010111
		len = 0x0001 + (len - 257);
		assert(0x01 <= len && len <= 0x17);
		bit_vec_push_nbits(v, len, 7);

		//TODO: implement dist
		return;
	}
	fprintf(stderr, "expected len between [257, 279] got len(%hu)", len);
}

// deflate (orchestrator)

/* 
 * we process len bytes of the stream, 
 * lookahead - len get processed in deflate_close()
 * to emit a final header 
 * */
size_t block0(u8 *stream, size_t len, pHandler emit, DeflateStatus status);
size_t block1(u8 *stream, size_t len, BitVec *v, pHandler emit, DeflateStatus);



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
	// TODO: refactor to avoid call repetition inside ifs
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

/*
DeflateCtx *deflate_ctx_init(u8 *stream, size_t len, pHandler f) {
	if (stream == NULL || f == NULL || len == 0) {
		fprintf(stderr, "Error: deflate_ctx_init failed due to invalid arguments (stream=%p, handler=%p, len=%zu)\n", 
				(void*)stream, (void*)f, len);
		return NULL; // Note: Use NULL instead of 0 for pointer return types
	}
	
	DeflateCtx *ctx = malloc(sizeof(DeflateCtx));
	if(!ctx) return NULL;
	ctx->pStream = stream;
	ctx->len = len;
	ctx->pEmit = f;
	return ctx;
};
*/
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

size_t block1(u8 *stream, size_t len, BitVec *v, pHandler emit, DeflateStatus status) {
	u64 header = (0x01 << 1) | (status == FINISH); // BFINAL=1 BTYPE=01
	bit_vec_push_nbits(v, header, 3);
	for(size_t i = 0; i < len; i++) {
		if(push_literal(v, stream[i]) == OUT_OF_BOUNDS) {
			//emit(bit_vec_data(v), bit_vec_byte_len(v));
			//bit_vec_clear(v);
			//assert(push_literal(v, stream[i]) == SUCCESS);

		}
	}
	
	push_eob(v);
	//This works
	//bit_vec_push_nbits(v, 0x0, 1);
	//bit_vec_push_nbits(v, 0x0, 1);
	//bit_vec_push_nbits(v, 0x0, 1);
	//bit_vec_push_nbits(v, 0x0, 1);
	//But padding 4 bits in a single operation seems to fail
	bit_vec_pad(v);
	emit(bit_vec_data(v), bit_vec_byte_len(v));
	//push_literal(v, stream[2]);
	// TODO: implement bit_vec_reset, bit_vec_pad_to_byte
	////backreference: <257, 3> EOB
	//stream[0] = 0x4b;
	//stream[1] = 0x4c;
	//// block: a a a b 257 3 256
	//stream[4] = 0x30;
	//stream[5] = 0x02;
	//stream[6] = 0x00;
	//emit(&header, sizeof(header));
	//emit(stream, len);
	//bit_vec_print(v);
	return len;
}
