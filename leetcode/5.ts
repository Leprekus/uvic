function longestPalindrome(s: string): string {
	/*
	 * if s is a set, i have to find all possible subset of s
	 * and return the longest subset s.t s = s^R
	 *
	 * case 1: none match are a palindrome so any random letter satisfies output
	 * case 2: we comes across the same char in which case any # of chars is a palindrome
	 * case 3: we need the nirror image of char
	 * */
	const m = s.length;
	const A = new Array(m).fill(new Array(m).fill(0));
	const comparator = (s1: string, s2: string, i: number, j: number) => (
	 //* case 1: none match are a palindrome so any random letter satisfies output
	
	 //* case 2: we comes across the same char in which case any # of chars is a palindrome
	(s1[i - 1] === s2[j]) ||
	 //* case 3: we need the mirror image of char
	 
	);
	for(let i = 0; i < m; i++){
		for(let j = 0; j < m; j++){
			if(s[i] === s[j]) A[i][j] = A[i - 1][j - 1] + 1;
			else A[i][j] = A[i - 1][j - 1];
			
		}
	
	}
};
