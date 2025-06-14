#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*callback)(const char *, size_t);
void parseFile(const char *path, callback cb);
void tokenizeStr(const char *str, char *buff, size_t buffLen, char *del, size_t delLen);

