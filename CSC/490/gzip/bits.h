#ifndef BITS_H
#define BITS_H

#include "types.h"
#include <stddef.h>

typedef struct bArray bArray;
typedef enum {
	SUCCESS,
	OUT_OF_BOUNDS,
} bStatus;
bArray *b_array_init(size_t numBytes);
bStatus b_array_push(bArray *a, u8 bit);
void b_array_print(bArray *a);
u8 reverse_u8(u8 n);
u16 reverse_u16(u16 n);
u32 reverse_u32(u32 n);
#endif
