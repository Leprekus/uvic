#include <string.h>
#include <stdio.h>
#include <stdlib.h>

FILE *fileOpen(const char *);
void parseFile(const char *);
void processLine(const char *);
void tokenizeStr(const char *, char *, size_t, char *, size_t);
void safeStrcpy(const char *, char *, size_t);

FILE *fileOpen(const char *path) {
	FILE *fptr = fopen(path, "r");
	if(!fptr) {
		perror("reader() fopen()");
		exit(EXIT_FAILURE);
	}
	return fptr;
}

void safeStrcpy(const char *src, char *buff, size_t buffLen) {
	/* ensure at least 1 extra space for null terminator*/
	size_t strLen = 0; while( strLen < buffLen - 1 && src[strLen] != '\0') strLen++; 

	if(strLen >= buffLen) { /* exit if no extra space for null terminator*/
		perror("safeStrcpy");
		exit(EXIT_FAILURE);
	}
	for(size_t i = 0; i < strLen; i++) buff[i] = src[i];
	for(size_t i = strLen; i < buffLen; i++) buff[i] = '\0';
}
/* 
 * takes an array of delimiters as input and replaces matching
 * characters in string with '\0' 
 * */
void tokenizeStr(const char *str, char *buff, size_t buffLen, char *del, size_t delLen){
	safeStrcpy(str, buff, buffLen);
	for(size_t i = 0; i < buffLen; i++){
		for(size_t j = 0; j < delLen; j++){
			if( buff[i] == del[j] )
				buff[i] = '\0';
		}
	}
	/*
	 * TODO: 
	 * i) parse 24_735_503 edges
	 * ii) store in HT
	 *
	 * */
}

void processLine(const char *line){
	size_t tokensLen = 128;
	char tokens[tokensLen]; tokenizeStr(line, tokens, tokensLen, "\t-", 2);
	for(size_t i = 0; i < tokensLen; i++)
		if(tokens[i] != '\0') putchar(tokens[i]);
	putchar('\n');

}

void parseFile(const char *path) {
	FILE *f = fileOpen(path);

	size_t lineLen = 128;
	char line [lineLen];
	

	int count = 0;
	while( fgets(line, (int)lineLen, f) ){
		processLine(line);
		if(count >= 1000) break;
		else count++;
	}
	fclose(f);
}
