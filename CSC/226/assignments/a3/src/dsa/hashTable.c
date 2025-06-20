#include "../include/dsa/hashTable.h"
#include "../include/utils/reader.h"
#include <stdint.h>

void htInit(HashTable *, Arena *, uint32_t);
uint32_t murmurhash (const char *, uint32_t, uint32_t);
uint32_t h(const char *, uint32_t);
Entry *htNewEntry(Arena *, Edge *);
void htInsert(HashTable *, Arena *, const char *, Edge *);
Entry *htGet(HashTable *, const char *);

void htInit(HashTable *ht, Arena *a, uint32_t capacity){
	ht->capacity = capacity;
	ht->size = 0;
	ht->count = 0;
	ht->alpha = 0;
	ht->entries =  NEW(a, Entry *, capacity);
}

//taken from: https://github.com/jwerle/murmurhash.c/blob/master/murmurhash.c
//example use:
//const char *key = "kinkajou"; // // 0xb6d99cf8
//uint32_t hash = murmurhash(key, (uint32_t) strlen(key), seed);
uint32_t murmurhash (const char *key, uint32_t len, uint32_t seed) {
  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;
  uint32_t r1 = 15;
  uint32_t r2 = 13;
  uint32_t m = 5;
  uint32_t n = 0xe6546b64;
  uint32_t h = 0;
  uint32_t k = 0;
  uint8_t *d = (uint8_t *) key; // 32 bit extract from `key'
  const uint32_t *chunks = NULL;
  const uint8_t *tail = NULL; // tail - last 8 bytes
  int i = 0;
  int l = (int)len / 4; // chunk length

  h = seed;

  chunks = (const uint32_t *) (d + l * 4); // body
  tail = (const uint8_t *) (d + l * 4); // last 8 byte chunk of `key'

  // for each 4 byte chunk of `key'
  for (i = -l; i != 0; ++i) {
    // next 4 byte chunk of `key'
  #if MURMURHASH_HAS_HTOLE32
    k = htole32(chunks[i]);
  #else
    k = chunks[i];
  #endif

    // encode next 4 byte chunk of `key'
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;

    // append to hash
    h ^= k;
    h = (h << r2) | (h >> (32 - r2));
    h = h * m + n;
  }

  k = 0;

  // remainder
  switch (len & 3) { // `len % 4'
    case 3: k ^= (tail[2] << 16);
	__attribute__((fallthrough));
    case 2: k ^= (tail[1] << 8);
	__attribute__((fallthrough));

    case 1:
      k ^= tail[0];
      k *= c1;
      k = (k << r1) | (k >> (32 - r1));
      k *= c2;
      h ^= k;
  }

  h ^= len;

  h ^= (h >> 16);
  h *= 0x85ebca6b;
  h ^= (h >> 13);
  h *= 0xc2b2ae35;
  h ^= (h >> 16);

  return h;
}
uint32_t h(const char *k, uint32_t capacity){
	uint32_t seed = 0x9e3779b9; // retrieved from https://softwareengineering.stackexchange.com/questions/402542/where-do-magic-hashing-constants-like-0x9e3779b9-and-0x9e3779b1-come-from
	uint32_t hash1 = murmurhash(k, (uint32_t) safeStrLen(k), seed);
	return hash1 % capacity;
}

Entry *htNewEntry(Arena *a, Edge *v){
	Entry *e = NEW(a, Entry, 1);
	e->edge = v;
	e->next = NULL;
	return e;
}
void htInsert(HashTable *ht, Arena *a, const char *k, Edge *v){
	uint32_t idx = h(k, ht->capacity);
	Entry *e = htNewEntry(a, v);

	//case 1: insert to empty slot
	//and update size
	if(ht->entries[idx] == NULL){
		ht->entries[idx] = e;
		ht->size++;
	} else {//case 2: append item to tail

		Entry *oldHead = ht->entries[idx]; 
		e->next = oldHead;
		ht->entries[idx] = e;
		
	}
	ht->count++;
	ht->alpha = (float)ht->count/(float)ht->capacity;

}


Entry *htGet(HashTable *ht, const char *k) {
	uint32_t idx = h(k, ht->capacity);
	if(!ht->entries[idx]) return NULL;
	Entry *curr = ht->entries[idx];
	while(curr) {

		if (memcmp(k, curr->edge->from, 16) == 0) return curr;	
		else curr = curr->next;
	}
	return NULL;
}
