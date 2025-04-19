typedef struct {
	int size;
	int capacity;
	int *data;
} Vec;

void vec_init(Vec *v, int capacity);
void vec_push(Vec *v, int n);
