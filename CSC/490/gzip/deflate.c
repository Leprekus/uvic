#include "deflate.h"
#include "bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// huffman


// lzss

// deflate (orchestrator)
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
	
	// STATE
	size_t written;
};
typedef u8 bool;
#define false 0
#define true 1
/* 
 * we process len bytes of the stream, 
 * lookahead - len get processed in deflate_close()
 * to emit a final header 
 * */

size_t block0(u8 *stream, size_t len, pHandler emit, DeflateStatus status);
size_t block1(u8 s[], size_t start, size_t end, pHandler);

DeflateCtx *deflate_ctx_init(
	int FD, u8 *stream, size_t len, pHandler pre, pHandler emit) {
	if(!stream || len < 2 || !emit) {
		fprintf(stderr, "Error invalid value: stream(%p), len(%zu), or emit(%p)", (void *)stream, len, (void *)emit);
		return NULL;
	}

	DeflateCtx *ctx = malloc(sizeof(DeflateCtx));
	if(!ctx) return NULL;
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
				deflate(ctx->stream + s1, n1, ctx->emit, RUN);	
			
			s1 ^= s2; e1 ^= e2; n1 = n2;
			s2 ^= s1; e2 ^= e1; n2 = 0;
			s1 ^= s2; e1 ^= e2;

		} else if(n2 < 0) return n2;
		else if(buffer_has_bytes(n2)){
			//printf("buffer_has_bytes s1(%zu), e1(%zu), s2(%zu), e2(%zu), n1(%zd), n2(%zd)", s1, e1, s2, e2, n1, n2);
			ctx->pre(ctx->stream + s1, n1);
			ctx->written += 
				deflate(ctx->stream  + s1, n1, ctx->emit, RUN);
			ctx->pre(ctx->stream + s2, n2);
			ctx->written += 
				deflate(ctx->stream  + s2, n2, ctx->emit, FINISH);
			break;
		}
		/* if buffer 2 is empty only deflate buf 1 */
		else {

			//printf("buffer_is_empty s1(%zu), e1(%zu), s2(%zu), e2(%zu), n1(%zd), n2(%zd)", s1, e1, s2, e2, n1, n2);
			ctx->pre(ctx->stream + s1, n1);
			ctx->written +=
				deflate(ctx->stream + s1, n1, ctx->emit, FINISH);
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
size_t deflate(u8 *stream, size_t read, pHandler emit, DeflateStatus status) {
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
