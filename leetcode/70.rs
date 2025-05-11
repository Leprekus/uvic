struct Solution {}
impl Solution {
    pub fn climb_stairs(n: i32) -> i32 {
        let mut prev = 0;
        let mut curr = 1;
        let mut ret  = 0; 
        for _ in 1..n + 1 {
            ret  = prev + curr;
            prev = curr;
            curr = ret;
        }

        ret

        
    }
}
