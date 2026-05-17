#include "deflate.h"
#include "bits.h"
#include <stdio.h>
#include <string.h>
// huffman


// lzss

// deflate (orchestrator)
size_t block0(u8 s[], size_t start, size_t end, pHandler f, u8 is_last);
size_t block1(u8 s[], size_t start, size_t end, pHandler);
/*
 * Entry point, so stream should always start at 0.
 * idea: calculate entropy as a heuristic to choose block type
 * https://en.wikipedia.org/wiki/Entropy_(information_theory)
 * H(X) := sum(p(x)) * ln(p(x))/ln(2)
 * */
/* deflate(bitstream) -> decompressed | huffman | huffman + lzss */
size_t deflate(u8 stream[], size_t len, pHandler f) {
	if(stream == NULL || f == NULL || len == 0) {
		perror("misuse: stream or handler is null or length is 0");
		return 0;
	}
	size_t a =  block0(stream, 0, 1, f, 0);
	size_t b =  block0(stream, 1, 2, f, 0);
	size_t c =  block0(stream, 2, 3, f, 1);
	return a + b + c;
}

void deflate_close(size_t written, pHandler f) {
	// occurs when we create an empty gzip
	if(written == 0) {
		block0(NULL, 0, 0, f, 0);
		return;
	}
}
size_t block0(u8 s[], size_t start, size_t end, pHandler f, u8 is_last) {
	size_t size = end - start;
	Block0 b = {
		.OPTIONS = is_last, 
		.LEN = size,
		.NLEN = ~size
	};
	u8 b_stream[sizeof(b)];
	memcpy(b_stream, &b, sizeof(b));

	f(b_stream, sizeof(Block0));
	f(s + start, size);
	return size;
}
