
#include <stddef.h>
#include <stdio.h>
typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer x, size_t len) {
	for(int i = 0; i < len; i++) //print in hex format with at least 2 digits
		printf(" %.2x", x[i]);
	putchar('\n');
}

void show_int(int x) {
	show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float(float x) {
	show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void *x) {
	show_bytes((byte_pointer)&x, sizeof(void *));
}

int main() {
	
	/*
	 *  we only care about looking at the address of x
	 *  and the size of that address
	 * */
	int x = 69;
	show_int(x);

	float y = 69.420;
	show_float(y);

	void *z;
	show_pointer(z);

	return 0;
}
