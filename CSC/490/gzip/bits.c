#include "utils.h"
#include "bits.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

struct BitVec {
	u8  *data; // array of bytes
	size_t capacity; // total bit capacity
	size_t written; // total bits written
};

BitVec *bit_vec_init(size_t nbits){
       BitVec *v = malloc(sizeof(BitVec));
       if(!v) return NULL;
       /**
	* Allocate a buffer with a capacity of 'nbits'.
	* Uses ceiling division ((nbits + 7) / 8) to ensure that if nbits is not 
	* a multiple of 8, we still allocate enough space for the final partial byte.
	*/       
       size_t nbytes = (nbits + 7) >> 3;
       u8 *data = malloc(sizeof(u8) * nbytes);
       if(!data) {
               free(v);
               return NULL;
       }
       v->capacity = nbits;
       v->written = 0;
       v->data = data;
       return v;
};
u8 bit_set(u8 n, u8 i){
	n |= (1 << i);
	return n;
}
u8 bit_clear(u8 n, u8 i){
	u8 mask = ~(1 << i);
	n &= mask;
	return n;
}
/* convert written bits to byte index: compute (written/8)*/
static inline size_t byte_idx(BitVec *v) {
	return v->written>>3;
}
/* compute the next available slot to push bits into */
static inline size_t bit_idx(BitVec *v) {
	return v->written&7; // 0 <= idx <= 7
}
BITVEC_STATUS bit_vec_push_bit(BitVec *v, u8 bit){
	bit = !!bit; // cast to 0 or 1
	// converts bytes to bits and check if written = capacity
	if(v->written == v->capacity) return FULL;
	
	v->data[byte_idx(v)] &= ~(1<<bit_idx(v)); // clear bit
	v->data[byte_idx(v)] |=  (bit<<bit_idx(v)); // set bit
	v->written++;
	return SUCCESS;
}

/* private helper function, so space validation should be done by caller */
static inline size_t bit_write(BitVec *v, u64 *bits,  size_t n, size_t i) {
	u8 mask = (0xFF << bit_idx(v));  
	// clear bits from the free slot up to the MSB in the byte (idx..7)
	v->data[byte_idx(v)] &= ~mask; 
	// push bits(idx..7) where LSB = idx and MSB = 7
	v->data[byte_idx(v)] |= (bits[i] << bit_idx(v)); 
	bits[i] = (bits[i] >> bit_idx(v)); // consume pushed bits
	size_t byte_capacity = 8 - bit_idx(v); 
	size_t written = (n < byte_capacity) ? n : byte_capacity;
	return written;
}
BITVEC_STATUS bit_vec_push_nbits(BitVec *v, u64 bits, size_t n) {
	if(n == 0) return SUCCESS;
	size_t capacity = v->capacity-v->written;
	if(n > capacity) return OUT_OF_BOUNDS;
	// tracks bits written, but it gets used as a byte_idx (i/8)
	size_t i = 0;    
	do {
		size_t written = bit_write(v, &bits, n, i>>3);	
		n -= written; i += written; v->written += written;
		//if n is not empty, and we wrote < 7 bits we must flush
		if(n && written < 7) {
			written = bit_write(v, &bits, n, i>>3);
			n -= written; i += written; v->written += written;
		}
	} while(n);
	return SUCCESS;

}

void bit_vec_print(BitVec *v) {
	if(v->written == 0) return;
	size_t n = byte_idx(v), i = 0;
	do { printf("[%#04X]\n", v->data[i]); i++; } while(n--);

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
