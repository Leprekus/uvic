#include "bits.h"
#include "deflate.h"
#include "gzip_spec.h"
#include "io_buf.h"
#include "stream.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

/*
 * stream(s) ->io_buffered_write(s)
 * */
void pre(void *s, size_t len);
void emit(void *s, size_t len);
WriteBuf *out_buf;
u32 crc = UINT32_MAX;
#define STREAM_LEN 32768
int main() {
	//INIT
	static u8 test[3]; 

	u8 *stream = stream_init(STREAM_LEN);
	GzHeader gz_h = gz_header_init();
	out_buf = io_buf_init();
	DeflateCtx *ctx = deflate_ctx_init(
			STDIN_FILENO, stream, STREAM_LEN, pre, emit);
	if(!out_buf || !ctx) exit(1);
	// WRITE HEADER
	io_buffered_write(out_buf, &gz_h, sizeof(gz_h));
	//MAIN LOOP TODO: change to 32768
	//crc = crc32_standard(crc, stream, bytes_read);
	//ssize_t written = process(STDIN_FILENO, stream, STREAM_LEN, &crc, io_out_cb);
	ssize_t written = deflate_read(ctx);
	if(written < 0) exit(1);
	GzFooter gz_f = { .ISIZE = written, .CRC32 = /*0x22444246*/~crc };
	io_buffered_write(out_buf, &gz_f, sizeof(gz_f)); //gzF.CRC32 ^= 0xFFFFFFFF; // <- FINAL xor at the end
	io_flush(out_buf);
	//printf("num: %#04zX rev: %#0zX\n", x, theOneAndOnlyRuleYouShouldNeverForgetWhenPushingNumericalValuesIntoAGZIPContainer(x));
}
void pre(void *s, size_t len) {
	crc = crc32_standard(crc, s, len);
}
void emit(void *s, size_t len){

	io_buffered_write(out_buf, s, len);
}
