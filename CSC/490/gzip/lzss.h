#include "types.h"
#include <stddef.h>


typedef struct { size_t i, length, distance; } BackRef;
typedef struct {
	enum { LITERAL, BACKREF } kind;
	union {
		u8 literal;
		BackRef ref;
	};
} LzssData;

typedef void (*pLzssStream)(LzssData d);
void lzss_compress_stream(u8 *s,  size_t len, pLzssStream emit);
/* Tables for deflate from PKZIP's appnote.txt. */
static unsigned border[] = {    /* Order of the bit length code lengths */
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
static u16 cplens[] = {         /* Copy lengths for literal codes 257..285 */
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
        /* note: see note #13 above about the 258 in this list. */
static u16 cplext[] = {         /* Extra bits for literal codes 257..285 */
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99}; /* 99==invalid */
static u16 cpdist[] = {         /* Copy offsets for distance codes 0..29 */
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577};
static u16 cpdext[] = {         /* Extra bits for distance codes */
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
        12, 12, 13, 13};

/*
 * Compact representation of the distance code value (0-31), distance range and number
 of extra bits to use in LZ77 compression (See Section 3.2.5 of RFC 1951)
distance_code_ranges = [
            [0,0,1,1],         [1,0,2,2],          [2,0,3,3],           [3,0,4,4],           [4,1,5,6],
            [5,1,7,8],         [6,2,9,12],         [7,2,13,16],         [8,3,17,24],         [9,3,25,32],
            [10,4,33,48],      [11,4,49,64],       [12,5,65,96],        [13,5,97,128],       [14,6,129,192],
            [15,6,193,256],    [16,7,257,384],     [17,7,385,512],      [18,8,513,768],      [19,8,769,1024],
            [20,9,1025,1536],  [21,9,1537,2048],   [22,10,2049,3072],   [23,10,3073,4096],   [24,11,4097,6144],
            [25,11,6145,8192], [26,12,8193,12288], [27,12,12289,16384], [28,13,16385,24576], [29,13,24577,32768],
    ]
 *
 * */
void lzss_find_pairs(const char *s,  size_t len);
