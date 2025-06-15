/*
M[u] = [(edge, weight)]
M[v] = [(edge, weight)]
Edge { edge, weight }
*/
#include <stdint.h>
#include "./graph.h"
typedef struct {
	uint32_t size; //number of slots available
	uint32_t count;//number of items stored 
	uint32_t capacity; //total number of slots
	float alpha; //count/capacity
	Edge **edges; //array of pointers to our edge's ptrs
} HashTable;
