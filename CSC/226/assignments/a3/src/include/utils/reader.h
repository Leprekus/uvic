#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * usage: the ith call to EXTRACT_TOKEN retrieves the ith tokens
 * takes a tokenized buffer, a buffer to copy the ith token to, and counters i, j
 * i: indexes the tokens buffer
 * j: indexes the buffer to store the token
 *
 * */
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


typedef void (*callback)(const char *, size_t);
void parseFile(const char *path, size_t lineLen, callback cb);
void tokenizeStr(const char *str, char *buff, size_t buffLen, char *del, size_t delLen);
size_t safeStrLen(const char *src);
void safeStrcpy(const char *src, char *buff, size_t buffLen);

