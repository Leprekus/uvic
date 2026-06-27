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

typedef struct __attribute__((__packed__)){
	u32 CRC32;
	u32 ISIZE;
} GzFooter;

GzHeader gz_header_init();
GzFooter gz_footer_init();
