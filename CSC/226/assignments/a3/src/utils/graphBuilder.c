#include "../include/utils/reader.h"
#include "../include/utils/graphBuilder.h"

void processLine(const char *, size_t);
void processLine(const char *line, size_t _){

	(void)_;
	size_t tokensLen = 128;
	/* tokenized with delimiters: \t, - 
	 * token form: start \0 weight \0 end \0 weight
	 * */
	char tokens[tokensLen]; tokenizeStr(line, tokens, tokensLen, "\t-", 2);
	
	for(size_t i = 0; i < tokensLen; i++)
		if(tokens[i] != '\0') putchar(tokens[i]);
	putchar('\n');

}
