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

GzFooter gzFooterInit() {
	 return (GzFooter){
		.CRC32 = 0x00000000,
		.ISIZE = 0x00000000
	};
}
