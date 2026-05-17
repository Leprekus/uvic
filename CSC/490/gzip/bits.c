#include "utils.h"
#include "bits.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

struct bArray {
	size_t size;  // capacity of bytes
	size_t BCount; // current byte count
	u8  bCount; // bit count for current byte 
	u8  *data; // array of bytes
};

bArray *b_array_init(size_t numBytes){
	bArray *a = malloc(sizeof(bArray));
	if(!a) return NULL;
	u8 *data = malloc(sizeof(u8) * numBytes);
	memset(data, 0, numBytes);
	if(!data) {
		free(a);
		return NULL;
	}
	a->size = numBytes;
	a->BCount = 0;
	a->bCount = 0;
	a->data = data;
	return a;
};
bStatus b_array_push(bArray *a, u8 bit){
	bit = !!bit; // cast to 0 or 1
	// buffer full
	if(a->BCount == a->size) return OUT_OF_BOUNDS;	
	//push the bit into current byte
	a->data[a->BCount] |= (bit << a->bCount); // update bit
	a->bCount = (a->bCount + 1) % 8;
	//move to next byte if current's full
	if(a->bCount == 0) {
		a->BCount++;
	}
	return SUCCESS;
}

void b_array_print(bArray *a) {
	for(size_t i = 0; i < a->size; i++) {
		if(i % 8 == 0) {
			putchar('\n');
		}
		u8 byte = a->data[i];
		printf("[%#04X] ", byte);
	}

}
// Source - https://stackoverflow.com/a/2602885
// Posted by sth, modified by community. See post 'Timeline' for change history
// Retrieved 2026-05-09, License - CC BY-SA 4.0
u8 reverse_u8(u8 b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

u16 reverse_u16(u16 n) {
	return reverse_u8(n) | (reverse_u8(n >> 8));
}
u32 reverse_u32(u32 n) {
	return (reverse_u8(n)) | (reverse_u8(n >> 8)) | (reverse_u8(n >> 16)) | (reverse_u8(n) >> 24);
}
