#include "huffman.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
	if(n1->val < n2->val) return 1;
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

/* appends without preserving sort */
int HQueue_push(HTree *T, Node *n) {
	if(T->queue.count == T->queue.size) {
		fprintf(stderr, "HQi havueue_add fail queue is full");
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
void _HTree_populate_codes(Node *n, u16 code, u16 size) {
    if (!n) return;

    if (!n->L && !n->R) {
	n->huffman.code = code;
	n->huffman.size = size;

        return;
    }
    assert(size < 32);
    assert(size < size + 1);
    _HTree_populate_codes(n->L, code << 1, size + 1);
    _HTree_populate_codes(n->R, (code << 1) | 1, size + 1);
}
void HTree_build(HTree *T) {
	
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
	T->root = HQueue(T)->head[0];
	_HTree_populate_codes(T->root, 0, 0);
}
void HTree_sort_codes(HTree *T) {
	size_t node_count = HPool(T).count;
	HQueue_destroy(T);
	HQueue_new(T, node_count);
	// push n - 1 elements
	for(size_t i = 0; i < node_count - 1; i++) {
		Node *n = &HPool(T).head[i];
		HQueue_push(T, n);
	}
	// push nth element and sort the list once
	HQueue_insert(T, &HPool(T).head[node_count - 1]);
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
