#include "types.h"
typedef struct __attribute__((__packed__)){
	u8 MAGIC1;
	u8 MAGIC2;
	u8 CM;
	u8 FLAGS;
	u32 MTIME;
	u8 XFL;
	u8 OS;
} GzHeader;

typedef struct __attribute__((__packed__)) {
	u16  OPTIONS; // IS_LAST, BTYPE, PADDING
	//u16 LEN;
	//u16 NLEN;
	//u8  BITSTREAM[BUF_LEN]; 
	//u32 COUNT; // counts bytes in BITSTREAM 
} Block;

typedef struct __attribute__((__packed__)){
	u32 CRC32;
	u32 ISIZE;
} GzFooter;

GzHeader gzHeaderInit();
Block blockInit();
GzFooter gzFooterInit();
