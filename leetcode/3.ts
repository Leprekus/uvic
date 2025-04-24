/*
 * the goal of the if statement is to place the L pointer
 * right after the first repeated char to begin counting
 * a new subsequence
 * */
function lengthOfLongestSubstring(s: string): number {
	const M = {};
    let L = 0;
    let R = 0;
    const n = s.length;
    let longest = 0;
    while( R < n ){
        const char = s[R];
        if( char in M )
            L = Math.max(L, M[char] + 1);

        M[char] = R;
        const length = R - L + 1; //we assume to have 1 character + current substr's length
        longest = Math.max(longest, length); //either current is longest or new length is longer
        R++;
    }
	return longest;
}
