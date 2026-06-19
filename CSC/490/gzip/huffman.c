#include "huffman.h"
#include "package_merge.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const size_t CODE_SIZE = 15;
struct HTree{
	Node *root;
	struct Pool {
		Node *head;
		size_t size;
		size_t count;
	} pool;
	struct Queue {
		Node **head;
		size_t size;
		size_t count;

	} queue;
};


void *alloc(size_t size) {
	void *p = malloc(size);
	if(!p) {
		fprintf(stderr, "alloc failed");
		exit(1);
		return NULL;
	}
	return p;
}

int comp(const void *p1, const void *p2) {
	Node *n1 = *(Node **)p1;
	Node *n2 = *(Node **)p2;
	if(n1->freq > n2->freq) return -1;
	if(n1->freq < n2->freq) return 1;

	if(n1->val > n2->val) return -1;
	if(n1->val <= n2->val) return 1;
	return 0;
}
void sort(Node **q, size_t size){
     qsort (q, size, sizeof(Node *), comp);
}
Node *HNode_new(HTree *T) {
	if(T->pool.count >= T->pool.size) return NULL;
	Node *n = &T->pool.head[T->pool.count++];
	n->L = NULL;
	n->R = NULL;
	n->freq = 0;
	n->val = 0;
	return n;
}
struct Pool HPool(HTree *T) { return T->pool; }
void HPool_clear(HTree *T) {
	T->pool.count = 0;
}
void HQueue_destroy(HTree *T) {
	if(T->queue.head == NULL) return;
	free(T->queue.head);
	T->queue.head = NULL;
	T->queue.count = 0;
	T->queue.size = 0;
}
void HQueue_new(HTree *T, size_t count) {
	// copy pointers pool to queue 
	Node **q = alloc(sizeof(Node *) * count);
	T->queue.head = q;
	T->queue.size = count;
	T->queue.count = 0;
}


int HQueue_len(HTree *T) { return T->queue.count; }
/* appends without preserving sort */
int HQueue_push(HTree *T, Node *n) {
	if(T->queue.count == T->queue.size) {
		//fprintf(stderr, "HQi havueue_add fail queue is full");
		return 0;
	}
	assert(T->queue.count < T->queue.size);
	T->queue.head[T->queue.count++] = n;
	return 1;
}
/* preserves sorting 
 * popping the elements returns them in desc order */
int HQueue_insert(HTree *T, Node *n){
	if(!HQueue_push(T, n)) return 0;
	sort(T->queue.head, T->queue.count);
	return 1;
}
Node *HQueue_pop_min(HTree *T){
	if(T->queue.count == 0) {
		//fprintf(stderr, "HQueue_pop_min fail queue is empty");
		return NULL;
	}
	assert(T->queue.count > 0);
	return T->queue.head[--T->queue.count];
}
HTree *HTree_new(u16 *data, size_t size) {
	HTree *T = alloc(sizeof(HTree));
	T->root = NULL;

	size_t count = 0;
	// count non-zero frequencies  
	for(size_t i = 0; i < size; i++)
		if(data[i] > 0) count++;
		
	// allocate resource pool
	Node *pool = alloc(sizeof(Node) * 2 * count);
	T->pool.head = pool;
	T->pool.size = 2 * count;
	T->pool.count = 0;

	HQueue_new(T, count);	
	// initialize queueu a sorted queue, 
	// to ensure correctness before building hfTree 
	for(size_t i = 0; i < size; i++) {
		if(data[i] == 0) continue;
		Node *n = HNode_new(T);
		n->val = i;
		n->freq = data[i];
		HQueue_insert(T, n);
	}
		 
	sort(T->queue.head, count);
	assert(T->queue.size == T->queue.count);
	//assert(T->queue.head[0] == T->pool.head);
	assert(T->pool.size == count * 2);
	assert(T->pool.count == count);
	return T;
}
 struct Queue *HQueue(HTree *T) {
	return &T->queue;
}
void _HTree_populate_codes(Node *n, size_t code, u16 size) {
    if (!n) return;

    if (!n->L && !n->R) {
	n->hf.code = code;
	n->hf.len = size;

        return;
    }

    assert(size < size + 1);
    _HTree_populate_codes(n->L, code << 1, size + 1);
    _HTree_populate_codes(n->R, (code << 1) | 1, size + 1);
}

void _huffman_method(HTree *T){
	while(HQueue(T)->count > 1) {
		assert(HQueue(T)->count <= HQueue(T)->size);
		assert(HPool(T).count <= HPool(T).size);
		Node *a = HQueue_pop_min(T);
		Node *b = HQueue_pop_min(T);
		Node *n = HNode_new(T);
		assert(a != NULL);
		n->L = a;
		n->R = b;
		n->freq = a->freq + b->freq;
		n->val = (a->val < b->val) ? a->val : b->val;
		int rc = HQueue_insert(T, n);
		assert(rc > 0);	
	}
}



void _HTree_get_nodes(Node *n, Node *nodes[], size_t *idx, size_t cap) {
    assert(n != NULL);
    if (!n) return;

    if (!n->L && !n->R) {
        assert(idx != NULL);
	    assert((*idx) < cap);
	    nodes[(*idx)++] = n;
        return;
    }

    _HTree_get_nodes(n->L, nodes, idx, cap);
    _HTree_get_nodes(n->R, nodes, idx, cap);
}

void _HTree_canonicalize_codes(HTree *T) {
	assert(T->root != NULL);
	size_t node_count = T->queue.size;
	Node *nodes[512];
    size_t idx = 0;
 	_HTree_get_nodes(T->root, nodes, &idx, T->queue.size);
	assert(node_count < 512);
	size_t max_len = 0; 
	for(size_t i = 0; i < node_count; i++){
		size_t len = nodes[i]->hf.len;
		max_len = (max_len > len) ? max_len : len;
	}
	assert(max_len < 512);
	// 1. count length frequency
	size_t lengths_count[max_len + 1]; 
	memset(lengths_count, 0, sizeof(lengths_count));
	for(size_t i = 0; i < node_count; i++)
		lengths_count[nodes[i]->hf.len]++;
	
	
	// 2. find the value of the smallest code for each code length
	size_t code = 0;
	lengths_count[0] = 0;
	size_t next_code[max_len + 1];
	for(size_t bits = 1; bits < max_len + 1; bits++) {
		code = (code + lengths_count[bits - 1]) << 1;
		next_code[bits] = code;
	}
	
	// 3. assign numerical values to all codes using consecutive values for all codes of the same length
	for(size_t i = 0; i < node_count; i++) {
		size_t len = nodes[i]->hf.len;
		size_t code = next_code[len];

		if(len != 0) {
			size_t tmp = code;
			size_t size = 0; while(tmp > 0) { tmp >>= 1; size++; }
			//printf("code(%zu) size(%zu)\n", code, size);
			T->queue.head[i]->hf.code = code;
			T->queue.head[i]->hf.len = size;
			next_code[len]++;
		}
	}

}
void HTree_build(HTree *T) {
    
	_huffman_method(T); assert(HQueue(T)->count == 1);
	T->root = HQueue(T)->head[0];
	_HTree_populate_codes(T->root, 0, 0);

    
    // populate histogram
    assert(T->queue.size < 512 && T->root != NULL);
    size_t size = T->queue.size;
    size_t idx = 0;
    Node *nodes[size]; _HTree_get_nodes(T->root, nodes, &idx, size); 
    u32 max_len = 0;
    for(size_t i = 0; i < size; i++) 
        max_len = (nodes[i]->hf.len>max_len) ? nodes[i]->hf.len : max_len;
    assert(max_len < 512);
    u32 histogram[max_len + 1]; memset(histogram, 0, sizeof(histogram));
    u8 code_lengths[size];
    for(size_t i = 0; i < size; i++)
        histogram[nodes[i]->hf.len]++;
    
    for(size_t i = 0; i < max_len + 1; i++) 
        printf("code_len_pre(%hu)\n", code_lengths[i]);
    packageMerge(CODE_SIZE, size, histogram, code_lengths);
    for(size_t i = 0; i < max_len + 1; i++) 
        printf("code_len_post(%hu)\n", code_lengths[i]);


	_HTree_canonicalize_codes(T);
}

void HTree_queue_leafs(HTree *T, Node *n) {
	if(!n->L && !n->R){
		HQueue_push(T, n);
		return;
	}
	HTree_queue_leafs(T, n->L);
	HTree_queue_leafs(T, n->R);
}
void HTree_sort_codes(HTree *T) {
	// Reset the queue count and push leafs into queue
	T->queue.count = 0;
	HTree_queue_leafs(T, T->root);
	assert(T->queue.count == T->queue.size);
	
	sort(T->queue.head, T->queue.count);
}

Node *HTree_pop_min(HTree *T) {
	return HQueue_pop_min(T);
}

void _HTree_print(Node *n, char *code, int depth) {
    if (!n) return;

    if (!n->L && !n->R) {
        code[depth] = '\0';
        printf("%#hx: %s\n", n->val, code);
        return;
    }

    code[depth] = '0';
    _HTree_print(n->L, code, depth + 1);

    code[depth] = '1';
    _HTree_print(n->R, code, depth + 1);
}
void HTree_print(HTree *T) {
	char code[256];
    	printf("Symbol |  Code\n");
	return _HTree_print(T->root, code, 0);	
}

void HTree_destroy(HTree *T){
	if(!T) return;
	if(T->pool.head) free(T->pool.head);
	HQueue_destroy(T);
	free(T);
}

void _cannonicalize_codes(size_t i_max_len, size_t i_max_code, u32 i_histogram[], u8 i_code_lengths[], u16 o_codes[]) {

    assert(i_max_len < 16);
    // 2. find smallest code for each length  
    u32 i;
    u32 code = 0;
    u32 next[i_max_len + 1];
    i_histogram[0] = 0;
    for (i = 1; i <= i_max_len; i++) {
        code = (code + i_histogram[i - 1]) << 1;
        next[i] = code;
    }

    // 3. now assign all codes
    for (i = 0; i <= i_max_code; i++) {
        u32 len = i_code_lengths[i];
        assert(len <= i_max_len);
        if(len != 0) {
            o_codes[i] = next[len];
            next[len]++;
        }
    }

}
/*
 * i_max_len - maximum length i.e 15 bits for deflate
 * i_max_len - size of i_histogram, o_code_lengths, o_codes
 * i_histogram - frequency count of symbols
 * o_code_lengths - empty array to store the computed lengths
 * o_codes - empty array containing the computed codes
 *
 * */
void compute_canon_hf_codes(
    u32 i_max_len, size_t i_max_code, u32 i_histogram[], u8 o_code_lengths[], u16 o_codes[]
) {
    
    int rc = packageMerge(i_max_len, i_max_code + 1, i_histogram, o_code_lengths);
    assert(o_code_lengths[i_max_code] > 0);
    assert(rc > 0);
    _cannonicalize_codes(i_max_len, i_max_code, i_histogram, o_code_lengths, o_codes);
}
