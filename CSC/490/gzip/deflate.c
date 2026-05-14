#include "deflate.h"
// huffman


// lzss

// deflate (orchestrator)
Block blockInit() {
	//TODO for empty gzip: 
	// why is value 0011 (BTYPE=1 and LAST=1) and not 0001 (BTYPE=0 and LAST=1)? 
	// why is BLOCK 2 Byte if options is just 1 byte?
	
}


size_t block0(u8 *s, size_t start, size_t end);
/*
 * idea: calculate entropy as a heuristic to choose block type
 * https://en.wikipedia.org/wiki/Entropy_(information_theory)
 * H(X) := sum(p(x)) * ln(p(x))/ln(2)
 * */
/* deflate(bitstream) -> decompressed | huffman | huffman + lzss */
void deflate(u8 *stream, size_t *written, size_t len) {
    /* case 1: we start a new stream */

    /* case 2: we have an ongoing stream */
    *written += block0(stream, 0, len);
}

void deflateClose(size_t written) {
}
size_t block0(u8 *s, size_t start, size_t end) {
    Block0 b = {
 		.OPTIONS = 0x03, // <- how to set opts correctly?
        .LEN = 0x0000,
        .NLEN = 0x0000,
        .BITSTREAM = 0x00,
        .COUNT = 0, 
	};

    return end - start;
}
