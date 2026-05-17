#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <stddef.h>
int itoa(int value, char *sp, int radix);
u32 crc32_standard(u32 crc, const u8 data[], size_t len);

#endif
