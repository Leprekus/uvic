/*
M[u] = [(edge, weight)]
M[v] = [(edge, weight)]
Edge { edge, weight }
*/

#pragma once
#include <stdint.h>
#include "./arena.h"
typedef struct Adjacent Adjacent;
struct Adjacent {
	char *to; //pointer to k of adjacent
	int w; //weight of edge 'from' -> 'to'
	struct Adjacent *next;
};
typedef struct Edge Edge;
struct Edge {
	char *from; // starting node
	Adjacent *adj; //linked list of nodes adjacent to from
};
typedef struct Entry Entry;
struct Entry {
	Edge *edge;
	Entry *next;

};
/*
 * Note: since HashTable uses chaining
 * the size may not match the count, and that is fine
 * */
typedef struct {
	uint32_t size; //number of slots occupied 
	uint32_t count;//number of items stored 
	uint32_t capacity; //total number of slots
	float alpha; //count/capacity
	struct Entry **entries; //array of pointers to our edge's ptrs
} HashTable;

void htInit(HashTable *ht, Arena *a, uint32_t capacity);
void htInsert(HashTable *ht, Arena *a, const char *k, Edge *v);
Entry *htGet(HashTable *ht, const char *k);

