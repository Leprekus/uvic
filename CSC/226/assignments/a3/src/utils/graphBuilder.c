#include "../include/utils/reader.h"

void processLine(const char *);
void processLine(const char *line){
	size_t tokensLen = 128;
	/* tokenized with delimiters: \t, - 
	 * token form: start \0 weight \0 end \0 weight
	 * */
	char tokens[tokensLen]; tokenizeStr(line, tokens, tokensLen, "\t-", 2);
	
	for(size_t i = 0; i < tokensLen; i++)
		if(tokens[i] != '\0') putchar(tokens[i]);
	putchar('\n');

}
