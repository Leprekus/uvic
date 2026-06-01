#ifndef BITS_H
#define BITS_H

#include "types.h"
#include <stddef.h>

typedef struct BitVec BitVec;
typedef enum {
	SUCCESS,
	FULL,
	OUT_OF_BOUNDS
} BITVEC_STATUS;
BitVec *bit_vec_init(size_t nbits);
u8 bit_set(u8 n, u8 i);
u8 bit_clear(u8 n, u8 i);
/*
 * parameters:
 * s - a stream of bytes
 * i - the ith byte in the stream
 * j - the jth bit in the ith byte
 * */
u8 bit_push_n(u8 s, size_t i, size_t j);
/*
 * parameters:
 * - BitVec *v
 * - array of bits to be pushed
 * - number of BITS to be pushed 
 * */
BITVEC_STATUS bit_vec_push_nbits(BitVec *v, u8 bits[], size_t n);
void bit_vec_print(BitVec *v);
u8 reverse_u8(u8 n);
u16 reverse_u16(u16 n);
u32 reverse_u32(u32 n);
#endif
