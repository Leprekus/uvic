//subarray number of distinct elements is equal to those in whole array
//min subarray len = # distinct elements
//anything in between = at least distinct elements + any extra
//max subarray len = whole array
//
//goal: generate all subarrays containing at least distinct elements


function countCompleteSubarrays(nums: number[]): number {
	const visited= {};
	let end = 0;
	for(let i = 0; i < nums.length; i++){
		const n = nums[i];
		if(!visited[n]) {
			visited[n] = 1;
			end = Math.max(end, i);
		}
	}

	let start = 0;
	while(start < end) {
		if(end < nums.length)
			end++;
		else {
			start++;
		}
	}
};
