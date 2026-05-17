#include "types.h"
#include <stddef.h>

// TODO: change to 4096
#define BUF_LEN 2

typedef struct WriteBuf WriteBuf;

WriteBuf *io_buf_init();
void io_flush(WriteBuf *buf);
// returns the new size of the buffer 
void io_buffered_write(WriteBuf *buf, const void *data, size_t size);
