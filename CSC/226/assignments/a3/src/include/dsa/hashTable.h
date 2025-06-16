/*
M[u] = [(edge, weight)]
M[v] = [(edge, weight)]
Edge { edge, weight }
*/
#include <stdint.h>
#include "./graph.h"
#include "./arena.h"

typedef struct Edge Edge;
struct Edge {
	const char *v; // starting node
	const char *u; //adjacent node
	const uint32_t w; // weight
};
typedef struct Entry Entry;
struct Entry {
	Edge **edges;
	Entry *next;

};
typedef struct {
	uint32_t size; //number of slots available
	uint32_t count;//number of items stored 
	uint32_t capacity; //total number of slots
	float alpha; //count/capacity
	struct Entry **entries; //array of pointers to our edge's ptrs
} HashTable;

void htInit(HashTable *ht, Arena *a, uint32_t capacity);
