function maxRepeating(sequence: string, word: string): number {

	
	//  rows represent sequence
	//  cols represent word
	//  so if word matches at any given part of sequence there will be
	//  a match at A[i][j]
	//  --------------
	// \		\
	// \		\
	// \		\
	// \		\
	// \		\
	// -------------
	const m = sequence.length, n = word.length;
	let count = 0, substr = 0;

	const A = new Array(m).fill(new Array(n).fill(0));
	A.fill(new Uint16Array(n));

	for(let i = 1; i < m; i++){
		for(let j = 1; j < n; i++){
			if(sequence[i] === word[j]) A[i][j] = A[i - 1][j - 1] + 1;
			
			if(count === word.length) substr++, count = 0;
		}
		
	}

};
