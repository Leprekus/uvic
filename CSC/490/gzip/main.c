#include "bits.h"
#include "deflate.h"
#include "gzip_spec.h"
#include "io_buf.h"
#include "stream.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * stream(s) -> bufferedWrite(s)
 * */
int main() {
	//INIT
	WriteBuf *outBuf = writeBufInit();
	if(!outBuf) exit(1);
	GzHeader gzH = gzHeaderInit();
	GzFooter gzF = gzFooterInit();
	// WRITE HEADER
	bufferedWrite(outBuf, &gzH, sizeof(gzH));
    //MAIN LOOP TODO: change to 32768
    Stream *stream = streamInit(32768);
    size_t written = 0;
    while(streamReadFrom(stream, STDIN_FILENO) > 0) {
        deflate(streamData(stream), &written, streamLength(stream));
	    //WRITE MEMBER
        bufferedWrite(outBuf, streamData(stream), streamLength(stream));
    }
    deflateClose(written);
	//WRITE FOOTER
	bufferedWrite(outBuf, &gzF, sizeof(gzF)); //gzF.CRC32 ^= 0xFFFFFFFF; // <- FINAL xor at the end
	
	flush(outBuf);
	//printf("num: %#04zX rev: %#0zX\n", x, theOneAndOnlyRuleYouShouldNeverForgetWhenPushingNumericalValuesIntoAGZIPContainer(x));
}
