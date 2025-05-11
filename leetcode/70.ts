function climbStairs(n: number): number {
   //const memo = {}; 
   let result = 0, prev = 0, curr = 0;
   for(let i = 0; i < n + 1; i++){

      /*
      if(i <= 1) result = 1;
      else result = memo[i - 1] + memo[i - 2];
      memo[i] = result;
      return memo[n];
      */

      result = 
         (1 & (i <= 1) as unknown as number) | 
         (prev + curr & ~(i <= 1));
      
      prev = curr;
      curr = result;
   }
   return result;

};


//console.log(climbStairs(5), climbStairs(10));
console.log(climbStairs(999999999));
