#include "deflate.h"
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
/* 
 * we process len bytes of the stream, 
 * lookahead - len get processed in deflate_close()
 * to emit a final header 
 * */
size_t block0(u8 *stream, size_t len, pHandler emit, DeflateStatus status);
size_t block1(u8 *stream, size_t len, BitVec *v, pHandler emit, DeflateStatus);
BITVEC_STATUS push_bits(BitVec *v, u64, size_t n, pHandler emit);
BITVEC_STATUS push_literal(BitVec *v, u16 literal, pHandler emit);
BITVEC_STATUS push_eob(BitVec *v, pHandler emit);

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
// parsing
// TODO: figure if i should unify push operations
BITVEC_STATUS push_literal(BitVec *v, u16 literal, pHandler emit) {
	if(0 <= literal && literal <= 143){ 
		// Seems to work
		// 0b0011_0000 through 0b1011_1111
		literal = 0x0030 + literal;
		assert(0x30 <= literal && literal <= 0xBF);
		return push_bits(v, reverse_u8(literal), 8, emit);
		
	}

	if(144 <= literal && literal <= 255) {
		// seems to work
		//0b110010000 through 0b111111111
		literal = 0x0190 + (literal - 144);
		//printf("literal(%#3hx) reversed(%#3hx)", literal, reverse_u16(literal));
		assert(0x0190 <= literal && literal <= 0x01FF);
		return push_bits(v, reverse_u16(literal)>>7, 9, emit);
	}

	fprintf(stderr, "expected literal between [0, 143] or [144, 255] got literal(%hu)", literal);
	return !SUCCESS;
};
BITVEC_STATUS push_eob(BitVec *v, pHandler emit) {
	return push_bits(v, 0x00, 7, emit);
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
	u64 header = (0x02) | (status == FINISH); // BFINAL=1 BTYPE=01
	push_bits(v, header, 3, emit);
	for(size_t i = 0; i < len; i++) {
		assert(push_literal(v, stream[i], emit)==SUCCESS);
	}
	
	// TODO: fix dirty emit()
	// if any of the push_bit functions emits and then
	// we emit again at the end of this function we get duplicated data
	assert(push_eob(v, emit) == SUCCESS);
	if(status == FINISH) bit_vec_pad(v);
	emit(bit_vec_data(v), bit_vec_byte_count(v));
	//bit_vec_print(v);
	return len;
}
