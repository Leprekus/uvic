function swap(A: number[], aL: number, a: number, B: number[], bL: number, b: number): void {
	[A, B] = [B, A];	
	[aL, bL] = [bL, aL]; 
	[a, b] = [b, a];
}

function findKthSmallestList (nums1: number[], nums2: number[], K:number): number {
	let A: number[] = nums1;
	let aL: number = 0;
	let a: number = nums1.length;

	let B: number[] = nums2;
	let bL: number = 0;
	let b: number = nums2.length;

	
	let top = Math.min(nums1.length, nums2.length);
	for(let i = 0; i < top; i++){
		//if A is longer than B send A to RHS and B to LHS
		if(a > b) swap(A, aL, a, B, bL, b);
					

		//if A has no elements the the k'ths smallest lies in B
		if(a === 0) return B[bL + K - 1];
		if(K === 1) return Math.min(A[aL], B[bL]);
		
		//get indices for Kth smallest
		const aK = Math.min(a, Math.ceil(K/2));
		const bK = K - aK;

		//if Kth smallest is in A make aL be the index of Kth smallest
		//update A's length, and K
		if(A[aL + aK - 1] < B[bL + bK - 1]) {
			aL += aK;
			a  -= aK;

			K  -= aK;
		}
		//do the same but for B
		else {
			bL += bK;
			b  -= bK;

			K  -= bK;
		}
	}
	return Math.min(A[aL], B[bL]);
}
function findMedianSortedArrays(nums1: number[], nums2: number[]): number {
	const lenSum = nums1.length + nums2.length;
	const ret = findKthSmallestList(nums1, nums2, Math.ceil(lenSum/2));	
	if(lenSum % 2) return ret;
	return (ret + findKthSmallestList(nums1, nums2, (lenSum + 2)/2)) /2;

};
