#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	int size;
	int capacity;
	int *data;
} Vec;

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
void factor(Vec *v, int n) {
	for(int i = 2; i * i <= n; i++) { // why check for i^2?
		while( n % i == 0 ) { //while i | n
			vec_push(v, i);
			n /= i;
		}
	}
	if( n > 1 ) vec_push(v, n);
}
int main(int argc, char **argv) {
	int x = atoi(argv[1]);
	Vec v;

	vec_init(&v, 10);
	factor(&v, x);	

	for(int i = 0; i < v.size; i++)
		printf("%d ", v.data[i]);
	printf("\n");
	return 0;
}
