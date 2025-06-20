#include "../include/dsa/unionFind.h"
#include "../include/dsa/arena.h"

void ufAdd(Arena *, const Edge *);
Node *ufFind(Node *);
void ufUnion(Node *, Node *);

void ufAdd(Arena *a, const Edge *e){
	Node *n = NEW(a, Node, 1);	
	n->edge = e;
	n->parent = n;
	n->size = 1;
}

Node *ufFind(Node *n){
	if(n->parent != n){
		return ufFind(n->parent);
	}
	return n->parent;
}

void ufUnion(Node *a, Node *b){
	Node *parentA = ufFind(a);
	Node *parentB = ufFind(b);

	if(parentA->parent == parentB->parent) return;

	if(parentA->size < parentB->size){
		parentA->parent = parentB->parent;
		parentB->size += parentA->size;
	} else {
		parentB->parent = parentA->parent;
		parentA->size += parentB->size;
	}

}
