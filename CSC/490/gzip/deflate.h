#include "types.h"
#include <stddef.h>
#include <stdbool.h>

#define BITSTREAM_LEN 4096

typedef struct __attribute__((__packed__)) {
	u8  OPTIONS; 
	u16 LEN;
	u16 NLEN;
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

/*
 * parameters:
 * - stream: stream of data to be processe (deflated0
 * - len: number of bytes in stream
 * - pHandler: 
 *   This callback is invoked one or more times as compressed data becomes 
 *   available. The engine may provide data in small fragments (e.g., headers 
 *   only) or large blocks depending on internal buffering. 
 *
 *   The handler is responsible for processing or persisting the data 
 *   immediately, as the buffer pointed to by the second argument may be 
 *   reused or invalidated once the callback returns.
 *   	arguments:
 *   	- a. u8 * -> u8 [] processed byte stream
 * 	- b. size_t -> number of bytes in the processed stream (a.)
 *
 * the io_out_cb allows the caller to handle the processed data as they wish.
 * */
typedef void (*pHandler)(u8 [], size_t);
size_t deflate(u8 stream[],  const size_t len, pHandler f);
void deflate_close(size_t written, pHandler f);

