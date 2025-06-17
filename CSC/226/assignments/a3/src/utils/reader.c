#include "../include/utils/reader.h"

FILE *fileOpen(const char *);
void parseFile(const char *, size_t lineLen, callback cb);
void tokenizeStr(const char *, char *, size_t, char *, size_t);
size_t safeStrLen(const char *);
void safeStrcpy(const char *, char *, size_t);

FILE *fileOpen(const char *path) {
	FILE *fptr = fopen(path, "r");
	if(!fptr) {
		perror("reader() fopen()");
		exit(EXIT_FAILURE);
	}
	return fptr;
}

size_t safeStrLen(const char *src){
	size_t strLen = 0; while( src[strLen] != '\0') strLen++; 
	return strLen;
}
void safeStrcpy(const char *src, char *buff, size_t buffLen) {
	/* ensure at least 1 extra space for null terminator*/
	
	size_t strLen = safeStrLen(src);
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



void parseFile(const char *path, size_t lineLen, callback cb) {
	FILE *f = fileOpen(path);

	char line [lineLen];
	

	int count = 0;
	while( fgets(line, (int)lineLen, f) ){
		cb(line, lineLen);
		//TODO: delete 1000 limit
		if(count >= 10000) break;
		count++;
	}
	fclose(f);
}
