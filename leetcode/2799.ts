
/*
 * SOLUTION"
 * nodeforece: https://leetcode.com/problems/count-complete-subarrays-in-an-array/solutions/6684309/0-ms-jsc-beats-10000-time-10000-memory-b-slq0/
 * */

const frequency = new Uint16Array(2001);
const add = (x: number):number => {
	if(++frequency[x] === 1) return 1;
	return 0;
};
const del = (x: number): number => {
	if(--frequency[x] === 0) return 1;
	return 0;
};

const countCompleteSubarrays = (A: number[]) => {
	frequency.fill(0);
	
	let distinct = 0;
	let numOfSubarrays = 0;
	const len = A.length;
	for(let i = 0; i < len; distinct += add(A[i++])); 

	frequency.fill(0);
	for(let i = 0, j = 0, distinct_prime = 0; ; ){
		if(distinct_prime < distinct){
			if(j === len) break;
			distinct_prime += add(A[j++]);
		} else {
			numOfSubarrays += len - j + 1;
			distinct_prime -= del(A[i++]);
		}
	}

	return numOfSubarrays;
}; 
