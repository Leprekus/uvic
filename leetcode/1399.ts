function countLargestGroup(nums: number): number {
    const M = {};
    let entry;
    let max = 0;
    let groups = 0;
    for(let n = 1; n <= nums; n++) {
         if(n < 10)
            entry = n;
         else {
            // let sum = n % 10; //grab n's LSB
            // let temp = n / 10 | 0; //remove n's LSB to access the next LSB
            // do {
            //     sum += (temp % 10); //grab n's LSB
            //     temp /= 10; //move to next LSB
            // } while(temp > 10)
            // entry = sum;
            let sum = n % 10;
            let temp = n;
            while(temp >= 10){
                temp = (temp / 10) | 0;
                sum += temp % 10;
            }
            entry = sum;
         }
        M[entry] = (M[entry] || 0) + 1;
        max = Math.max(max, M[entry]);
    }
    for(const v of Object.values(M)) 
        if(v === max) groups++;
    
    return groups;
};
