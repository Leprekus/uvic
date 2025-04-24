#include <stddef.h>
#define MAX(x, y)( (( (x) > (y) ) > 0) ? (x) : (y))

typedef struct {} HT;
unsigned h(char k){

}
unsigned ht_get(HT *ht, char k);
unsigned ht_insert(HT *ht, char K);
void ht_clear();
int lengthOfLongestSubstring(char* s) {
    size_t len = 0; while( s[len] != '\0') len++;
    char substring[len];
    unsigned curr = 0;
    unsigned longest = 0;

    HT ht;
    for(size_t i = 0; i < len; i++){
    	if( !ht_get(&ht, s[i]) ){
		ht_insert(&ht, s[i]);
		curr++;
	} else {
		ht_insert(&ht, s[i]);
		curr = 1;
		longest = MAX(curr, longest);
	}
    }
    return 0;
    
}

int main() {
	return 0;
}
