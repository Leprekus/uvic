#include "../include/dsa/arena.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Arena newArena(ptrdiff_t);
void *alloc(Arena *, ptrdiff_t, ptrdiff_t, ptrdiff_t);

Arena newArena(ptrdiff_t cap){
	if(cap < 0) {
		perror("newArena");
		exit(EXIT_FAILURE);
	}
	Arena a = {0};
	void *buff = malloc((size_t)cap);
	if( !buff ){
		perror("newArena");
		exit(EXIT_FAILURE);
	}
	a.beg = buff;
	a.end = a.beg + cap;
	return a;
}

void *alloc(Arena *a, ptrdiff_t size, ptrdiff_t align, ptrdiff_t count){
	
	//get the number of bytes to advance to the next alignment
	uintptr_t beg = (uintptr_t)a->beg;
	uintptr_t pad = (-beg) & (uintptr_t)(align - 1);
	ptrdiff_t padding = (ptrdiff_t) pad;
	//get available memory after taking padding into account
	ptrdiff_t available = a->end - a->beg - padding;
	//ensure we have enough memory and that it is
	//large enough to store our item COUNT times
	if(available < 0 || count > available/size){
		perror("alloc: failure");
		exit(EXIT_FAILURE);
	}
	void *p = a->beg + padding; //create ptr to aligned address
	a->beg += padding + count * size; //update next free 
	return memset(p, 0, (size_t)(count * size)); // return ptr to the beginning of allocated memory (set to 0)
}
