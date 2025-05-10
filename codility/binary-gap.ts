function solution(N: number): number {
	//Convert N to the number of bits needed to hold N. this is important
	//because it ensures cur visits each bit of N once.
	const top = Math.ceil(Math.log(N)/Math.log(2)); 
	
	//Invert bits so it becomes easier to count 0's which are now 1's.
	const flipped = ~N;
	//start at the 0th position (LSB)
	let cur = 0b01;
	let count = 0;
	let maxCount = 0;
	for(let i = 0; i < top; i++, cur<<=1){
		
		//Count if the current position is a 1 
		let isGap = ((cur & flipped) > 0) as unknown as number; //1 0 0 1 -> 0 1 1 0	
		count += isGap;
		//Since we want to count the gap between 1's (which are now 0's)
		//we count all the 1's and only 'save' them to maxCount when we are
		//no longer at a gap (which means we hit a 1)
		//we can assume we were previously at a gap as we were counting 0's
		//
		//only edge case not taken into account is the following: 10000 as this would incorrectly count the 0's
		if(!isGap){
			maxCount = maxCount > count ? maxCount : count;
			count = 0;
		}
	}
	return maxCount
}
console.log('expected 2, got: ', solution(9));
console.log('expected 4, got: ', solution(529));
console.log('expected 2, got: ', solution(20));
console.log('expected 0, got: ', solution(32));
