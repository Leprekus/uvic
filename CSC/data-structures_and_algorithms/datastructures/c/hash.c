#include <string.h>

//polynomial rolling hash function:
//where s = string, p = prime, m = modulo
//hash is equal to the sum of:
// ( s[0]*p^0 + s[1]*p^1 + s[2]*p^2 + ... + p[n-1]*p^(n-1) ) % m
long long hash(const char *string){
	//prime rougly equal to the number of inputs our hash may receive:
	//a-z
	const int p = 31;
	//modulo (a large prime), hash range is from [0, m)
	const int m =  1e9 + 9;
	long long value = 0;
	long long p_pow = 1;

	for(int i = 0; i < strlen(string); i++){
		//map our char to a numerical value starting at 1
		value = (value + (string[i] - 'a' + 1) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}
	return value;
}
