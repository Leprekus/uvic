#include "types.h"
#include <stddef.h>
#include <stdbool.h>

#define BITSTREAM_LEN 4096

typedef struct __attribute__((__packed__)) {
	u8  OPTIONS; 
	u16 LEN;
	u16 NLEN;
	u8  BITSTREAM[BITSTREAM_LEN]; 
	u32 COUNT; // counts bytes in BITSTREAM 
} Block0;

typedef enum {
    BLOCK0,
    BLOCK1,
    BLOCK2
} BTYPE;

typedef struct {
    size_t written;
    bool isDirty;
    BTYPE type;
} DeflateCtx;

void deflate(u8 *stream, size_t *written, const size_t len);
void deflateClose(size_t written);
