#include "gzip_spec.h"

GzHeader gzHeaderInit() {
	return (GzHeader) {
		.MAGIC1 = 0x1F,
                .MAGIC2 = 0x8B,
                .CM = 0x08,
                .FLAGS = 0x00, 
                .MTIME = 0x00,
                .XFL = 0x00,
                .OS = 0x03
	};
}

Block blockInit() {
	//TODO for empty gzip: 
	// why is value 0011 (BTYPE=1 and LAST=1) and not 0001 (BTYPE=0 and LAST=1)? 
	// why is BLOCK 2 Byte if options is just 1 byte?
	return (Block) {
 		.OPTIONS = 0x03, // <- how to set opts correctly?
                //.LEN = 0x0000,
                //.NLEN = 0x0000,
                //.BITSTREAM = 0x00,
                //.COUNT = 0, 
	};
}
GzFooter gzFooterInit() {
	 return (GzFooter){
		.CRC32 = 0x00000000,
		.ISIZE = 0x00000000
	};
}
