#include "bits.h"
#include "deflate.h"
#include "gzip_spec.h"
#include "io_buf.h"
#include "stream.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * stream(s) ->io_buffered_write(s)
 * */

void io_out_cb(u8 s[], size_t len);
WriteBuf *out_buf;
int main() {
	//INIT
	GzHeader gz_h = gz_header_init();
	out_buf = io_buf_init();
	if(!out_buf) exit(1);
	// WRITE HEADER
	io_buffered_write(out_buf, &gz_h, sizeof(gz_h));
	//MAIN LOOP TODO: change to 32768
	Stream *pS = stream_init(32768);
	size_t written = 0;
	u32 crc = UINT32_MAX;
	int bytes_read = 0;
	static u8 test[] = { 0xaa, 0xbb, 0xcc }; 
//	while((bytes_read = stream_read_from(pS, STDIN_FILENO)) > 0) {
//		size_t nbytes = deflate(
//			stream_data(pS), stream_length(pS), io_out_cb);
//		crc = crc32_standard(crc, stream_data(pS), stream_length(pS));
//		if(nbytes == 0 || bytes_read != nbytes) exit(1);
//		written += nbytes;
//		//WRITE MEMBER
//		}
	do{
		size_t nbytes = deflate(test, sizeof(test), io_out_cb);
		crc = crc32_standard(crc, test, sizeof(test));
		if(nbytes == 0) exit(1);
		written += nbytes;
	} while(0);
	if(bytes_read == -1) exit(1);
	deflate_close(written, io_out_cb);
	//WRITE FOOTER: CRC & UNCOMPRESSED SIZE
	GzFooter gz_f = { .ISIZE = written, .CRC32 = ~crc };
	io_buffered_write(out_buf, &gz_f, sizeof(gz_f)); //gzF.CRC32 ^= 0xFFFFFFFF; // <- FINAL xor at the end

	io_flush(out_buf);
	//printf("num: %#04zX rev: %#0zX\n", x, theOneAndOnlyRuleYouShouldNeverForgetWhenPushingNumericalValuesIntoAGZIPContainer(x));
}

void io_out_cb(u8 s[], size_t len){
	io_buffered_write(out_buf, s, len);
}
