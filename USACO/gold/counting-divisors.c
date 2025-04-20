#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
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

void factor(unsigned n, Vec *v){
	for(unsigned i = 2; i*i <= n; i++){
		while(n % i == 0) {
			vec_push(v, i);
			n /= i; //remove prime factor from n
		}
	

	}
	if(n > 1) vec_push(v, n); //count any remaining prime factors
};

unsigned get_divisors(const char *path, const unsigned num) {

	Hash ht;
	hash_init(&ht, num);
	
	Vec v;
	vec_init(&v, 100);

	factor(num, &v);

	unsigned count = 0;
	for(unsigned i = 0; i < v.size; i++){
		if( !hash_get(&ht, v.data[i]) ){
			hash_insert(&ht, v.data[i]);
			count++;
		}
		for(unsigned j = 0; j < v.size; j++){
			if(i == j) continue;
			unsigned num = v.data[i] * v.data[j];
			if( !hash_get(&ht, num) ){
				hash_insert(&ht, num);
				//printf("divisor %d\n", num);
				count++;
			}
			continue;
		}
	}
	count++; //count n | n
	count ++; //count 1 | n

	//printf("divisors %d", count);

	free(v.data);
	free(ht.data);

	return count;

}

unsigned *get_answers(const char *path) {
	size_t size;
	char *string = read_file(path, &size);

	size_t nums_size;
	unsigned *answers = process_file(string, &size, &nums_size);
	return answers;
}

void compare_answers(const char *input_path, const char *compare_path){
	size_t size;
	char *string = read_file(input_path, &size);

	size_t nums_size;
	unsigned *nums = process_file(string, &size, &nums_size);
	Vec ret;
	vec_init(&ret, nums_size);

	for(unsigned i = 0; i < nums_size; i++){
	 	vec_push(&ret, get_divisors(input_path, nums[i]));
		printf("vec %d\n", ret.data[ ret.size - 1 ]);
	}
	unsigned *answers = get_answers(compare_path);
	for(unsigned i = 0; i < nums_size; i++)
		printf("%s computed: %d, expected: %d\n", (ret.data[i] == answers[i]) ? "CORRECT" : "INCORRECT", ret.data[i], answers[i]);

	free(string);
	free(nums);
	free(answers);
	free(ret.data);
};
int main(int argc, char **argv) {

	if(argc < 2){
		printf("please provide input file");
		exit(1);
	}
	if(argc > 3){
		printf("usage: <program> <input_file> <compare_file>");
		exit(1);
	}

	compare_answers(argv[1], argv[2]);

	return 0;
}
