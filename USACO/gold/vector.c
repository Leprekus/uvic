#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

void vec_init(Vec *v, int capacity) {
	int *data = (int *)malloc(capacity * sizeof(int));
	if(data == NULL)
		exit(1);
	v->data = data;
	v->size = 0;
	v->capacity = capacity;
}

void vec_resize(Vec *v) {
	int new_capacity = v->capacity * 5/3;
	int *new_data = (int *)malloc( new_capacity * sizeof(int));
	if(new_data == NULL)
		exit(1);

	for(int i = 0; i < v->capacity; i++)
		new_data[i] = v->data[i];
	free(v->data);
	v->data = new_data;
	v->capacity = new_capacity;
	v->size = v->size;
}

void vec_push(Vec *v, int n) {
	if(v->capacity == v->size)
		vec_resize(v);
	v->data[ v->size++ ] = n;
}
