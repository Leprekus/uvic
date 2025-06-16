#include <stddef.h>

#include "utils/reader.h"
#include "utils/graphBuilder.h"
#include "dsa/graph.h"
#include "dsa/arena.h"
#include "dsa/hashTable.h"

Arena a; 
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

	EXTRACT_TOKEN(tokens, key, i, j, lineLen) j = 0;
	EXTRACT_TOKEN(tokens, value, i, j, lineLen) j = 0;

	

	for(size_t k = 0; k < lineLen; k++)
		if(tokens[k] != '\0') putchar(tokens[k]);
	putchar('\n');

}
int main(void) {
	/*
	 * ptrdiff_t is 8bytes long
	 * we want to allocate 4gb so 4e+9
	 * */
	a = newArena(4e+9);
	HashTable ht;
	htInit(&ht,  &a, 2.5e+8); //initialize a ht with a capacity of 250M items
	const size_t lineLen = 128;
	parseFile("../CC-Neuron_cci.tsv", lineLen, &processLine);
	return 0;

}
