#include <stddef.h>
#include <stdlib.h>

#include "utils/reader.h"
#include "utils/graphBuilder.h"
#include "dsa/graph.h"
#include "dsa/arena.h"

Arena a; 
Graph G;

void processLine(const char *, size_t);
void processLine(const char *line, size_t lineLen){

	/* tokenized with delimiters: \t, - 
	 * token form: start \0 weight \0 end \0 weight
	 * */
	char tokens[lineLen]; tokenizeStr(line, tokens, lineLen, "\t-", 2);
	
	char key[lineLen]; safeStrcpy(tokens, key, lineLen);
	char value[lineLen];
	size_t i = 0; //indexes tokens
	size_t j = 0; //indexes key / value
	

	
	EXTRACT_TOKEN(tokens, key, i, j, lineLen) j = 0;
	EXTRACT_TOKEN(tokens, value, i, j, lineLen) j = 0;
	Edge *e1 = graphAddVertex(&a, &G, key);
	int w1 = atoi(value);
	
		

	EXTRACT_TOKEN(tokens, key, i, j, lineLen) j = 0;
	EXTRACT_TOKEN(tokens, value, i, j, lineLen) j = 0;
	Edge *e2 = graphAddVertex(&a, &G, key);
	int w2 = atoi(value);



	graphAddEdge(&a, &G, e1, e2, w1);
	graphAddEdge(&a, &G, e2, e1, w2);

	//printf("e1) str: %s, v: %d\n", e1->start, e1->w);
	//printf("e2) str: %s, v: %d\n", e2->start, e2->w);

	//for(size_t k = 0; k < lineLen; k++)
	//	if(tokens[k] != '\0') putchar(tokens[k]);
	//putchar('\n');

}
int main(void) {
	/*
	 * ptrdiff_t is 8bytes long
	 * we want to allocate 4gb so 4e+9
	 * */
	a = newArena(4e+9);
	G = graphInit(&a, 49471033); //initialize a ht with a capacity of 250M items
	const size_t lineLen = 128;
	parseFile("../CC-Neuron_cci.tsv", lineLen, &processLine);
	printf("HT \n --------------------\n size: %u\n count: %u\n capacity %u\n alpha %f \n edges: %u\n", 
	 G.ht.size, //number of slots available
	 G.ht.count, //number of items stored 
	 G.ht.capacity,  //total number of slots
	 G.ht.alpha, //count/capacity
	 G.edgeCount

	);
	printf("arena mem space left: %ld\n", a.end - a.beg);
	return 0;

}
