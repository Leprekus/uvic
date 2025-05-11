function climbStairs(n: number): number {
   const memo = {}; 
   let result = 0;
   for(let i = 0; i < n + 1; i++){

      /*
      if(i <= 1) result = 1;
      else result = memo[i - 1] + memo[i - 2];
      */

      result = 
         (1 & (i <= 1) as unknown as number) | 
         (memo[i -1] + memo[i - 2] & ~(i <= 1));
      
      memo[i] = result;
   }
   return memo[n];

};


console.log(climbStairs(999999999));
