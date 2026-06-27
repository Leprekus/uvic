#include "types.h"
#include <stddef.h>


typedef struct { size_t i, length, distance; } BackRef;
typedef struct {
	enum TokenKind { LITERAL, BACKREF } kind;
	union {
		u8 literal;
		BackRef ref;
	};
} LzssData;

typedef void (*pLzssStream)(LzssData d);
void lzss_compress_stream(u8 *s,  size_t len, pLzssStream emit);

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
