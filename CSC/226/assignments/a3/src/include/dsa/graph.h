#include "./hashTable.h"

typedef struct Graph Graph; 
struct Graph {
	HashTable ht;
	uint32_t vertexCount;
	uint32_t edgeCount;
};
Graph graphInit(Arena *a, uint32_t capacity);
void graphAddEdge(Arena *, Graph *G, Edge *from, Edge *to, int w);
Edge *graphAddVertex(Arena *a, Graph *G, const char *key);

