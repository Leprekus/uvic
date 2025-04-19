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

void hash_insert(Hash *ht, unsigned k){

	unsigned i = h(k) % ht->capacity;
	unsigned count = 0;
	while( ht->data[i] && count < ht->capacity ){
		i = (h(k) + i*i) % ht->capacity;
		count++;
	}
	if(count >= ht->capacity) exit(1);
	ht->data[i] = k;
	
};
unsigned hash_get(Hash *ht, unsigned k){

	unsigned i = h(k) % ht->capacity;
	unsigned count = 0;
	while( ht->data[i] != k && count < ht->capacity ){
		i = (h(k) + i*i) % ht->capacity;
		count++;

	}
	if(ht->data[i] == k)
		return ht->data[i];

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

unsigned *process_file( const char *string, size_t *size, size_t *nums_size){

	//nums allocation
	unsigned *nums = (unsigned *)calloc(*size, sizeof(unsigned));
	unsigned n = 0;
	if(nums == NULL) exit(1);
	
	//string processing
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
	//ret allocation
	unsigned *ret = (unsigned *)calloc(n, sizeof(unsigned));
	if(ret == NULL) exit(1);
	for(unsigned i = 0; i < n; i++)
		ret[i] = nums[i];
	free(nums);
	*nums_size = n;
	return ret;
}

void factor(unsigned n, Hash *ht, unsigned *count){
	(*count)++; //count 1 as divisor of n
	for(unsigned i = 2; i*i <= n; i++){
		
		while(n % i == 0) {
		if(1){
		printf("ran ok i = %d", i);
		}
		
			
			if( !hash_get(ht, n) ){
				hash_insert(ht, n); 
				printf("ran ok n = %d", n);
				exit(0);

				(*count)++;
			}
			//vec_push(v, i); //store prime factor
			n /= i; //remove prime factor from n
		}
	

	}
	if(n > 1 && !hash_get(ht, n) ) (*count)++; //push any remaining prime factors
};
int main(int argc, char **argv) {
	size_t size;
	char *string = read_file(argv[1], &size);

	size_t nums_size;
	unsigned *nums = process_file(string, &size, &nums_size);

	unsigned count;
	Hash ht;
	hash_init(&ht, nums[0]);
	//factor(nums[1], &ht, &count);
	for(unsigned i = 2; i < nums[0]; i++){
		if(!hash_get(&ht, i)){
			hash_insert(&ht, i);
			printf("inserting... ");
		}
		printf("inserted i = %d retrieved ht(i) =  %d\n", i, hash_get(&ht, i));
	}

	free(string);
	free(nums);
	free(ht.data);
	return 0;
}
