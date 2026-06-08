#include "types.h"
#include "lzss.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

size_t idx = 0;
const size_t size = 32768;
BackRef *hash = NULL;

BackRef *hash_init(){
    BackRef *h = malloc(sizeof(BackRef) * size);
    if(!h) return NULL;
    hash = h;
    return hash;
}
int hash_set(BackRef ref){
    if(idx == size) return -1;
    hash[idx++] = ref;
    return 1;
}
 static BackRef Nil = {.i=0,.length=0,.distance=0};
BackRef hash_get(const u8 *s, BackRef want){
    for(int i = 0; i < idx; i++) {
        BackRef have = hash[i];
        size_t n = have.length,
        i1 = have.i, i2 = want.i;
        // compare both strings
        while(have.length == want.length && s[i1++] == s[i2++] && n--);
        if(n == 0) {
            return hash[i];
        }
    }
    return Nil;
}
void hash_clear(){
    idx = 0;
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
    hash_init();
    //const char *s = "matchamtchasdfasdvdsflongestmatchasdfasdfmatch_longestmatchavfdj";
    size_t longest_match = 0;
    
    // a. find and store all literals we can backreference
	for(size_t i = 0; i < len; i++){
		BackRef ref = { .i = 0, .length = 0, .distance = 0 };
		for(size_t j = i + 3; j <= len; j++) {
			size_t n = j - i;
		
		    // 1. find longest match for string at index i
			for(size_t k = j; k <= len - n; k++){
			//printf("k(%zu) <= len-n(%zu), len(%zu), n(%zu) = j(%zu) - i(%zu)", k, len-n, len, n, j, i);
			    // if there's a match and is longer for previous keep new max
			 
				if(memcmp(s + i, s + k, n) == 0 && n > ref.length){
				    
					ref.i = i; ref.length = n; ref.distance = k - j;
					if(ref.length > longest_match) longest_match = ref.length;
					//printf("i(%zu) len(%zu) dist(%zu)\n", i, n, ref.distance);
				}

			}
			
		}
		// 2. store longest match for the ith substring
		// if we have seen the substr, skip it (move ptrs to end of substr)
		// and continue
		if(ref.length > 0){
		    if(idx == size) printf("ERR-HASH-FULL");
		    hash_set(ref);
		    //printf("set i(%zu) len(%zu) \n", ref.i, ref.length);
		    //i += ref.length;
		}
	}
	
	// b. second pass to emit backreferences
	for(size_t i = 0; i < len; i++){
		BackRef best_match = Nil;
		for(size_t j = 3; j <= longest_match; j++) {
			size_t n = (j < len - j) ? j : (len - j);
			
		    BackRef match = hash_get(s,
		    (BackRef){.i=i,.length=n,.distance=0});
		    // check if the match:
		    // - is not Nil
		    // - isn't referencing itself
		    // - match must be before literal
		    int is_not_nil = match.length>0,
		        match_comes_before = match.i<i;
		    
		    if(
		       is_not_nil &&
		       match_comes_before &&
		       match.length > best_match.length
		        )
		        best_match = match;
		    
		}
		
		
		if(best_match.length > 0){
		    //emit_backreference
		    best_match.distance = i - best_match.i;
		    emit((LzssData){.kind=BACKREF, .ref=best_match});
		    // we want to point to the last character of the match
		    // so the next loop iteration points us to the 1st
		    // character AFTER the match
		    i += best_match.length - 1;
		    //printf(" new i(%zu)\n", i);
		    //emit literals
		} else emit((LzssData){.kind=LITERAL, .literal = s[i]});
		
	}
	if(hash) free(hash);
	hash_clear();
}
