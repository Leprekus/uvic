function solution(N: number): number {
    // Implement your solution here
    let isSaveable = 0;
    let i = 0b01;
    let count = 0;
    let maxCount = 0;
    while(i > 0){
        isSaveable += (N & i) > 0 as unknown as number;
	//start counting when we've seen at least a 1 to not overcount 0's
	if(isSaveable === 1) count += ((N & i) === 0) as unknown as number;
	//when we come another one save to maxCount and set isSaveable to 1
	//as the last 1 we see will count as the RHS bound for our gap
        if(isSaveable === 2){
        	maxCount = maxCount > count ? maxCount : count;
		isSaveable = 1;
		count = 0;
	
        }
        i<<=1;
    }
    return maxCount;
}

console.log('expected 2, got: ', solution(9));
console.log('expected 4, got: ', solution(529));
console.log('expected 1, got: ', solution(20));
console.log('expected 0, got: ', solution(32));
