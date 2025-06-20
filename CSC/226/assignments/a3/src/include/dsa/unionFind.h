#include "graph.h"
typedef struct UnionFind UnionFind;
typedef struct Node Node;
struct Node {
	Node *parent;
	const Edge *edge;
	uint32_t size;
	
};

void ufAdd(Arena *a, const Edge *e);
Node *ufFind(Node *n);
void ufUnion(Node *a, Node *b);

