#include "types.h"
#include "lzss.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	struct Bucket {
		size_t head; // index of the first entry in the original list
		size_t count;
	} bucket[256];
	struct Prefix { 
		// if true, prefix is backreferencing and cannot be referenced
		bool is_referencing;
		const u8 *head, *tail; // points to start and end of prefix
		size_t idx; // ith item in the bucket
	} *prefix; // contains prefixes of string[i..n]
	size_t size; // total prefix capacity
	size_t count; // current prefix items
} Table;
Table table;
/* create a table the size of the string */
struct Prefix *table_init(size_t size){
	struct Prefix *prefixes = malloc(sizeof(struct Prefix) * size);
	if(!prefixes) return NULL;
	memset(table.bucket, -1, sizeof(table.bucket));
	table.prefix = prefixes;
	table.size = size;
	table.count = 0;
	return prefixes;
}
/* store the address to the ith prefix of a string.
 * since this is a prefix table, it is assumed 
 * that table.size = strlen */
int table_set(const u8 *head, const u8 *tail){
	assert(head <= tail);
	if(table.count == table.size) return 0;
	table.prefix[table.count++] = (struct Prefix){
		.is_referencing = 0,
		.head = head,
		.tail = tail,
		.idx = 0
	};
	return 1;
}
int _prefix_comp(const void * pPre1, const void * pPre2) {
	// make shallow copies of the struct
	// this prevents the pointers stored
	// in table.prefix from being mutated
	struct Prefix p1 = *((struct Prefix *)pPre1);
	struct Prefix p2 = *((struct Prefix *)pPre2);
	// because tail = string[len-1] we need a +1 offset
	const u8 *p1_start = p1.head,
	         *p2_start = p2.head;
	// order entries by first character then index
	// such i.e abcde with index 1 and abcdefgx with index 0 are sorted:
	// 0 abcdefgx
	// 1 abcde
	if(*p1_start == *p2_start && p1_start < p2_start) return -1;
	if(*p1_start == *p2_start && p1_start > p2_start) return 1;
	if(*p1_start != *p2_start && *p1_start < *p2_start) return -1;
	if(*p1_start != *p2_start && *p1_start > *p2_start) return 1;
	return 0;
	/*
	size_t len1 = (p1.tail - p1.head) + 1;
	size_t len2 = (p2.tail - p2.head) + 1;
	size_t min_len = (len1 < len2) ? len1 : len2;
	while(
		min_len && 
		p1.head < p1.tail &&
		p2.head < p2.tail &&
		*p1.head == *p2.head
	){
		min_len--;
		p1.head++;
		p2.head++;
	};
	// Note
	// Note: -1 go first, 1 go next
	// same prefix different lenghts i.e abc < abcdef
	if(!min_len && p1_start < p2_start) return -1;
	if(!min_len && p1_start > p2_start) return 1;
	// if there's length remaining, biggest char first
	if (min_len && *p1.head < *p2.head) return -1;
	if (min_len && *p1.head > *p2.head) return  1;
	
	// prefixes are equal
	return 0;
	*/
}
/* 
 * sorts prefixes in descending order
 * maps bucket[i] to the first and longest prefix starting with i
 * counts number of items in the ith bucket
 * Bucket
 *
 * */
void table_map_buckets(){
	// sort
	qsort(table.prefix, table.size, sizeof(struct Prefix), _prefix_comp);
	// map the prefix to bucket entry and count frequency
	for(int i = 0; i < table.size; i++) {
		struct Prefix p = table.prefix[i];
		u8 c = *p.head; // read first char of ith prefix
		// if we haven't seen the prefix yet, store the location
		// where it first appears in table.prefix
		if(table.bucket[c].head == -1) {
			table.bucket[c].head = i;
			table.bucket[c].count = 1;
		} else table.bucket[c].count++;
		table.prefix[i].idx = table.bucket[c].count - 1;
		assert(table.prefix[i].idx != - 1);

		assert(table.bucket[c].head != - 1);
		assert(table.bucket[c].count != - 1);

	}
}
/* given a prefix i 
 * return the bucket containing the index and item count
 * for such prefix */
struct Bucket table_get(u8 i){
	return table.bucket[i];
}
void table_clear(){
	table.count = 0;
	memset(table.bucket, -1, sizeof(table.bucket));
}
void table_destroy() {
	if(table.prefix) free(table.prefix);
}

void table_print() {
	for(int i = 0; i < 256; i++)
		if(table.bucket[i].count != -1) printf("Bucket(%c) count %zu\n", i, table.bucket[i].count);
	// i refers to the index in the original list
	// b refers to the index within the bucket
	for(size_t i = 0; i < table.size; i++) {
		struct Prefix p = table.prefix[i];
		int len = p.tail - p.head;
		size_t idx = table.size - (p.tail - p.head);
		struct Bucket b = table_get(*p.head);
		
		
		printf("%zu) bidx(%zu) %.*s\n", idx, p.idx, len, p.head);
		// check pointers correctly point to end of array
		//assert(*table.prefix[i].tail == 'j');
	}
}

// returns number of packaged bytes
LzssData lzss_package(struct Prefix *e, u8 has_prev_entry){
	LzssData d = {
			.kind = LITERAL,
			.ref = (BackRef) {
				.distance = 0,
				.length = 0,
				.i = 0,
			}
		};
	if(!has_prev_entry) {
		d.kind = LITERAL;
		d.literal = *e->head;
		d.ref.length = 1;
		return d;
	};
	struct Prefix match = *(e - 1);
	struct Prefix entry = *e;

	// since there is a previous entry we know the first character matches
	// so we skip it
	match.head++; size_t n = 1;
	while(
		!match.is_referencing && // TODO: check if it is necessary to keep this, tests pass without it
		match.head < entry.head && 
		*match.head == *entry.head
		) {

		match.head++;
		entry.head++;
		n++;
	};
	if(n >= 3) { // update memory address of entry
		(e - 1)->is_referencing = 0xFF; 
		d.kind = BACKREF;
		d.ref.length = n;
		d.ref.distance = entry.head - match.head;
		d.ref.i = table.size - (entry.tail - entry.head);
		return d;
	}
	d.kind = LITERAL;
	d.literal = *e->head;
	d.ref.length = 1;
	return  d;
}


//TODO: 
//	- implement 32k lookback limit
//	- this is a feasible solution that needs 
//	improvement to handle larger inputs
// IDEAS:
// - Tries https://en.wikipedia.org/wiki/Suffix_tree
// - resources: https://stackoverflow.com/questions/88615/what-algorithm-can-you-use-to-find-duplicate-phrases-in-a-string
/* finds back references and pushes literals or a backrefernce to a string */
void lzss_compress_stream(u8 *s,  size_t len, pLzssStream emit){
	table_init(len);
	// a. build prefix table
	for(size_t i = 0; i < len; i++) {
		int rc = table_set(s + i, s + len - 1); 
		assert(rc > 0);	
	}
	assert(table.count == table.size);
	// b. sort prefixes and initialize buckts
	table_map_buckets();
	//table_print();

	// c. find matches using binary search and emit 
	// literals or backreferences
	size_t written = 0;
	for(size_t i = 0; i < len; i++) {
		u8 c = s[i];
		struct Bucket b = table_get(c);
		struct Prefix *pfx = table.prefix + b.head;
		// binary search for the ith stream prefix in the bucket 
		// backtrack to previous  matching entries in the bucket
		// we can backreference 
		size_t L = 0, R = b.count - 1;
		while(L <= R) {
			size_t M = (L + R) / 2;
			u8 is_self = (s + i) == pfx[M].head;
			u8 has_prev_entry = pfx[M].idx > 0;
			// if there is a match 
			if(is_self) {
				LzssData d =
					lzss_package(pfx + M, has_prev_entry);

				emit(d);

				i += (d.ref.length  - 1);
				written += d.ref.length;
				break;
			}
			else if (pfx[M].head < s + i) L = M + 1;
			else R = M - 1;

		}
		
	}
	table_destroy();
}
