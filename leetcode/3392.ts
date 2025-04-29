function countSubarrays(nums: number[]): number {
    const len = nums.length - 2;
    let count = 0;
    for(let i = 0; i < len; i++) {
        const sum = nums[i] + nums[i + 2];
        const second = nums[i + 1] /2;
        count += (sum === second) as unknown as number;
    }
    return count;
};
