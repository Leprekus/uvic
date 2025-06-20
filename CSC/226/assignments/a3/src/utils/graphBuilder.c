
#include "../include/utils/graphBuilder.h"
/*
 * takes in tokens (src) and buff (dest)
 * buff's len must be >= token's
 * tokens must start with a valid string as
 * the macro will skip any null terminated characters and break
 * otherwise it copies the string into buff
 * j is used as buff's index
 *
 * */

void kruskal(Arena, Graph *);
void boruvka(Graph *);
void jarnik(Graph *);

Edge **sortEdges(Arena *scratch, Graph *G, Edge **arr){

};
//since the arena is passed by copy the space occupied is implicitly freed upon return
void kruskal(Arena scratch, Graph *G){
	Edge **sorted = NEW(&scratch, Edge *, G->edgeCount); 
	sortEdges(&scratch, G, sorted);
}
void boruvka(Graph *G) {

}void jarnik(Graph *G) {

}
