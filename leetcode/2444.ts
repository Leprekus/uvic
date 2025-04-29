const merge = (L: number[], R: number[], min: number, max: number,  bounds: [i_min: number, j_max: number]) => {
	const res: number[] = [];
	let i = 0, j = 0;
	while(i < L.length && j < R.length){
		if(L[i] === min || R[j] === min)  bounds[0] = Math.min(bounds[0], res.length);
		if(L[i] === max || R[j] === max) bounds[1] = Math.max(bounds[1], res.length);
		if(L[i] < R[j]) res.push(L[i++]);
		else res.push(R[j++]);

	};
	while(i < L.length) {
		if(L[i] === min) bounds[0] = Math.max(bounds[0], res.length);
        	res.push(L[i++]);
    	}
	while(j < R.length) {
		if(R[j] === max) bounds[1] = Math.max(bounds[1], res.length);
		res.push(R[j++]);
	}
    console.log({ res })
	return res;
};
const mergeSort = (A: number[], min: number, max: number, bounds: [i_min: number, j_max: number]) => {
	if(A.length <= 1) return A;
	const mid = A.length / 2 | 0;
	let L = A.slice(0, mid);
	let R = A.slice(mid, A.length);

	L = mergeSort(L, min, max, bounds); 
	R = mergeSort(R, min, max, bounds);
	return merge(L, R, min, max, bounds);

	};

function countSubarrays(A: number[], minKnums: number, maxK: number): number {
	const frequency = {};
	const keys: number[] = [];	
	const add = (x: number):number => {
		if(frequency[x] === undefined) {
			frequency[x] = keys.length;
			keys.push(1);
		} else {
            const idx = frequency[x];
            keys[idx]++;
        }
        const idx = frequency[x];
		return (keys[idx] === 1) as unknown as number;
	};
	const del = (x: number): number => {
		const idx = frequency[x];
		if(--keys[idx] === 0) return 1;
		return 0;
	};
	
    
	
	let distinct = 0;
	let numOfSubarrays = 0;
	const bounds: [i_min: number, j_max: number] = [ Infinity, -Infinity ];
	const sorted = mergeSort(A, minKnums, maxK, bounds);
    bounds[1] += 1; //maxK must be inclusive
	let len = bounds[1] - bounds[0];
	for(let i = bounds[0]; i < bounds[1]; distinct += add(sorted[i++])); 
	keys.fill(0);
	for(let i = bounds[0], j = bounds[0], distinct_prime = 0; ;){
		if(distinct_prime < distinct) {
			if(j === bounds[1]) break;
			distinct_prime += add(sorted[j++]);
		} else {
			let j_offset = j - bounds[0];
			numOfSubarrays += len - j_offset + 1;
			distinct_prime -= del(sorted[i++]);
		}
	}
	return numOfSubarrays;


};




