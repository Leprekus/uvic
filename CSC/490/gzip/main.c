#include "bits.h"
#include "gzip_spec.h"
#include "io_buf.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
	//INIT
	WriteBuf *outBuf = writeBufInit();
	if(!outBuf) exit(1);
	GzHeader gzH = gzHeaderInit();
	Block b = blockInit();
	GzFooter gzF = gzFooterInit();
	//HEADER
	bufferedWrite(outBuf, &gzH, sizeof(gzH));
	//MEMBER
	bufferedWrite(outBuf, &b, sizeof(b));
	//FOOTER
	bufferedWrite(outBuf, &gzF, sizeof(gzF)); //gzF.CRC32 ^= 0xFFFFFFFF; // <- FINAL xor at the end
	
	flush(outBuf);
	//printf("num: %#04zX rev: %#0zX\n", x, theOneAndOnlyRuleYouShouldNeverForgetWhenPushingNumericalValuesIntoAGZIPContainer(x));
}
