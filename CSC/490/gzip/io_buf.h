#include "types.h"
#include <stddef.h>

// TODO: change to 4096
#define BUF_LEN 2

typedef struct WriteBuf WriteBuf;

WriteBuf *writeBufInit();
void flush(WriteBuf *buf);
// returns the new size of the buffer 
void bufferedWrite(WriteBuf *buf, const void *data, size_t size);
