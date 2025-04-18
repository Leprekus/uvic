#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	unsigned size;
	unsigned capacity;
	unsigned *data;
} Hash;

unsigned int h(unsigned int x) {
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = (x >> 16) ^ x;
	return x;
}

void hash_init(Hash *ht, unsigned capacity){

	unsigned *data = (unsigned *)calloc(capacity, sizeof(unsigned));
	if(data == NULL) exit(1);

	ht->data = data;
	ht->capacity = capacity;
	ht->size = 0;
}

void hash_insert(Hash *ht, unsigned k, unsigned v){

	unsigned i = h(k);
	unsigned count = 0;
	while( ht->data[i] && count < ht->capacity ){
		i = (h(k) + i*i) % ht->capacity;
		count++;
	}
	if(count >= ht->capacity) exit(1);
	ht->data[i] = v;
	
};
unsigned hash_get(Hash *ht, unsigned k){

	unsigned i = h(k);
	unsigned count = 0;
	while( ht->data[i] && count < ht->capacity ){
		i = (h(k) + i*i) % ht->capacity;
		count++;

		if(ht->data[i] == k)
			return ht->data[i];
	}
	return 0;
};

char* read_file(const char *path, size_t *size){
	FILE *f;
	f = fopen(path, "r");
	if( fseek(f, 0, SEEK_END) != 0 ) exit(1); //go to end of file
	*size = ftell(f); //tell position (# of lines)

	fseek(f, 0, SEEK_SET); //go to start of file

	char *string = (char *)malloc(*size * sizeof(char));
	if( string ) fread(string, 1, *size, f); //read file contents into array
	else exit(1);
	fclose(f);
	return string;
}

unsigned *process_file( const char *string, size_t *size ){

	unsigned *nums = (unsigned *)calloc(*size, sizeof(unsigned));
	unsigned n = 0;
	if(nums == NULL) exit(1);
	
	char line[7];
	unsigned l = 0;
	for(size_t i = 0; i < *size; i++){
		if(string[i] != '\n')
			line[l++] = string[i];
		else {
			nums[n++] = atoi(line);
			l = 0;
		}

	}
	return nums;
}
int main(int argc, char **argv) {
	size_t size;
	char *string = read_file(argv[1], &size);
	unsigned *nums = process_file(string, &size);

	for(int i = 0; i < size; i++)
		printf("%d\n", nums[i]);
	free(string);
	return 0;
}
