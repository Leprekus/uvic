#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "types.h"
#include <stddef.h>

typedef struct HTree HTree;
typedef struct Node {
	u16 val;
	size_t freq;
	struct _HF { size_t code, len; } hf;
	struct Node *L, *R;
} Node;
//Node *HNode_new(HTree *T);
//struct Pool *HPool(HTree *T);
//void HPool_clear(HTree *T);
//int HQueue_add(HTree *T, Node *n);
//Node *HQueue_pop_min(HTree *T);
int HQueue_len(HTree *T);
HTree *HTree_new(u16 *data, size_t size);
//struct Queue *HQueue(HTree *T);
void HTree_build(HTree *T);
void HTree_sort_codes(HTree *T);
Node *HTree_pop_min(HTree *T);
void HTree_print(HTree *T);
void HTree_destroy(HTree *T);
void compute_canon_hf_codes(
    u32 i_MAX_BITS, size_t i_size, u32 i_histogram[], u8 i_code_lengths[], u16 o_codes[]
);
#endif
