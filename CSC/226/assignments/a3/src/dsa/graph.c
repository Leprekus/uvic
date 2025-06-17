#include "../include/dsa/graph.h"
#include "../include/dsa/arena.h"
#include "../include/utils/reader.h"

Graph graphInit(Arena *, uint32_t);
void graphAddEdge(Arena *, Graph *, Edge *, Edge *, int w);
Edge *graphAddVertex(Arena *, Graph *, const char *);

Graph graphInit(Arena *a, uint32_t capacity) {
	Graph G = {0};
	HashTable ht;
	htInit(&ht, a, capacity);
	G.ht = ht;
	G.vertexCount = ht.count;
	return G;
}

/*
 * takes keys from two existing vertices in the graph to join them
 * */
void graphAddEdge(Arena *a, Graph *G, Edge *from, Edge *to, int w){
	if( !htGet(&G->ht, from->from) || !htGet(&G->ht, to->from) ){
		perror("graphAddEdge: vertex not found in G");
		exit(EXIT_FAILURE);
	}


	G->edgeCount++;
	Adjacent *adj = NEW(a, Adjacent, 1);
	adj->to = to->from;
	adj->w = w;
	adj->next = NULL;
	if(!from->adj) {
	 	from->adj = adj;
		return;
	}
	Adjacent *oldHead = from->adj;
	from->adj= adj;
	adj->next = oldHead;
}

Edge *graphAddVertex(Arena *a, Graph *G, const char *key){

	Entry *entry = htGet(&G->ht, key);
	if( entry ) return entry->edge;
	Edge *e= NEW(a, Edge, 1); //initialize Edge
	size_t keyLen = safeStrLen(key) + 1; //get key len
	e->from = NEW(a, char, (ptrdiff_t)keyLen); //Allocate space for the string
	safeStrcpy(key, e->from, keyLen); //copy key to from
	e->adj = NULL;

	htInsert(&G->ht, a, key, e);
	return e;

}

