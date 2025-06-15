#include <stddef.h>
typedef struct {
	char *beg;
	char *end;
} Arena;

// align should always be a power of 2
void *alloc(Arena *a, ptrdiff_t size, ptrdiff_t align, ptrdiff_t count);
