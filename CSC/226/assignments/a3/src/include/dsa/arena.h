#pragma once
#include <stddef.h>
typedef struct {
	char *beg;
	char *end;
} Arena;

//usage: NEW(Arena, Type, Count)
#define NEW(a, T, n) (T *)alloc(a, sizeof(T), _Alignof(T), n)

Arena newArena(ptrdiff_t cap);
// align should always be a power of 2
void *alloc(Arena *a, ptrdiff_t size, ptrdiff_t align, ptrdiff_t count);
