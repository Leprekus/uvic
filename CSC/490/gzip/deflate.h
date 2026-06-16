#include "types.h"
#include "bits.h"
#include <stddef.h>
#include <stdio.h>

#define BITSTREAM_LEN 4096

typedef enum { START, RUN, FINISH } DeflateStatus;

/*
 * parameters:
 * - stream: stream of data to be processed
 * - len: number of bytes in stream
 * - pHandler: 
 *   This callback is invoked one or more times as compressed data becomes 
 *   available. The engine may provide data in small fragments (e.g., headers 
 *   only) or large blocks depending on internal buffering. 
 *
 *   The handler is responsible for processing or persisting the deflated data 
 *   immediately, as the buffer pointed to by (u8 *stream) may be 
 *   reused or invalidated once the callback returns.
 *   	pHandler arguments:
 *   	- u8 *data -> deflated byte stream
 * 	- size_t len-> number of bytes in (u8 *data)
 *
 * the io_out_cb allows the caller to handle the processed data as they wish.
 * */

/*
 * The API offers two entrypoints to deflate:
 * 	- deflate_read - which is a wrapper around read() 
 * 	and uses deflate() internally. It handles reading
 * 	and deflating data from a File Descriptor until 
 * 	the input is exhausted.
 * 	- deflate - deflates the byte stream in (u8 *stream), uses the
 * 	status argument to determine when to emit the last header. By calling
 * 	this method the developer is in charge of correctly passing 
 * 	status = RUN when there will be subsequent calls to deflate()
 * 	and setting status = FINISH on the last call to deflate().
 * */
typedef struct DeflateCtx DeflateCtx;
typedef void (*pHandler)(void *data, size_t len);
DeflateCtx *deflate_ctx_init(
	int FD, u8 *stream, size_t len, pHandler pre, pHandler emit);
ssize_t deflate_read(DeflateCtx *ctx);
size_t deflate(u8 *stream, size_t read, BitVec *v, pHandler emit, DeflateStatus status);
void deflate_close(size_t written, pHandler f);

u16 static cl_symbol[] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, // literal 0..15
};
u16 static cl_repeat[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // repeat 0 for symbols 0..15
16, 16, 16, 16, 16, 16, // repeat

};

