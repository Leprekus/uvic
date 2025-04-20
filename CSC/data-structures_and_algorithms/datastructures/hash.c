/**
* assignment: insert keys: 10, 22, 31, 4, 15, 28, 17, 88, 59
* into a hash table of length N = 11
* using open addressing and the following hash functions:
* quadratic, linear, 
* and double: h_2(k) = 1 + (k mod(N-1))
*/

//open addressing: means we don't chain
//but rather each element occupies a space in the HT

#include <stdio.h>
#include <string.h>
int const N = 11;
int counter = 0;
int HT[11] = {0};
int keys[] = { 10, 22, 31, 4, 15, 28, 17, 88, 59 };

int h(int k) { return k % N; }

int quadratic_h(int k, unsigned int i){

	unsigned int idx = (h(k) + i*i) % N;
	if( HT[idx] == 0 || HT[idx] == k) {
		printf("SUCCESS: inserted %d, at index %d\n", k, idx);
		HT[idx] = k;
		return k;
	}
	printf("COLLISION: inserting %d, collided with %d at index %d\n", k, HT[idx], idx);
	return quadratic_h(k, ++i);
}

int l_i = 0;
int linear_h(int k, unsigned int i){
	unsigned int idx = h(k);
	if( HT[idx] == 0 || HT[idx] == k ) {
		printf("SUCCESS: inserted %d, at index %d\n", k, idx);
		HT[idx] = k;
		return k;
	}
	printf("COLLISION: inserting %d, collided with %d at index %d\n", k, HT[idx], idx);
	idx = (h(k) + ++i) % N;
	return linear_h(k, idx);
}

int double_h(int k, unsigned int i){
	unsigned int idx = h(i);

	if( HT[idx] == 0 || HT[idx] == k ) {
		printf("SUCCESS: inserted %d, at index %d\n", k, idx);
		HT[idx] = k;
		return k;
	}
	printf("COLLISION: inserting %d, collided with %d at index %d\n", k, HT[idx], idx);
	return linear_h(k, 1 + (k % (N-1)));
}


int main () {

	printf("QUADRATIC PROBING\n");
	for(int i = 0; i < 9; i++)
		quadratic_h(keys[i], i);
	putchar('\n');
	memset(HT, 0, N);
	
	printf("LINEAR PROBING\n");
	for(int i = 0; i < 9; i++)
		linear_h(keys[i], i);

	putchar('\n');
	memset(HT, 0, N);

	printf("DOUBLE PROBING\n");
	putchar('\n');
	memset(HT, 0, N);

	return 0;
}
