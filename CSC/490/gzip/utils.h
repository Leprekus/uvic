#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <stddef.h>
int itoa(int value, char *sp, int radix);
u32 crc32_standard(u32 crc, const u8 data[], size_t len);
size_t check_rle_run(size_t prev, size_t curr, size_t cap, size_t *contiguous);
#endif
