#include "utils/reader.h"
#include "utils/graphBuilder.h"

#define EXTRACT_TOKEN(tokens, buff, i, j, len)                 \
	for((i) = (i); (i) < (len); (i)++){\
		if( (tokens)[(i)] == '\0' ) { \
			while((i) < (len) - 1 && (tokens)[++(i)] == '\0'){} \
     			break;\
		}\
     		else {\
			(buff)[(j)++] = (tokens)[(i)];\
		}\
	} (buff)[(j)] = '\0';\

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
	printf("k2: %s\n", key);
	printf("v2: %s\n", value);

	

	for(size_t i = 0; i < lineLen; i++)
		if(tokens[i] != '\0') putchar(tokens[i]);
	putchar('\n');

}
int main(void) {
	const size_t lineLen = 128;
	parseFile("../CC-Neuron_cci.tsv", lineLen, &processLine);
	return 0;
}
