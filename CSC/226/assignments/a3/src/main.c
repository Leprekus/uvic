#include "utils/reader.h"
#include "utils/graphBuilder.h"

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
	printf("k2: %s\n", key);
	printf("v2: %s\n", value);

	

	for(size_t k = 0; k < lineLen; k++)
		if(tokens[k] != '\0') putchar(tokens[k]);
	putchar('\n');

}
int main(void) {
	const size_t lineLen = 128;
	parseFile("../CC-Neuron_cci.tsv", lineLen, &processLine);
	return 0;
}
