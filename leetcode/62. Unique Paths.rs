pub fn unique_paths(m: i32, n: i32) -> i32 {
    let m: usize = m as usize;
    let n: usize = n as usize;
    let mut grid = vec![vec![0; n]; m]; 
    grid[0][0] = 1; 
    
    for i in 0..m {
        for j in 0..n {
            if i > 0 {
                grid[i][j] += grid[i-1][j];
            }
            if j > 0 {
                grid[i][j] += grid[i][j-1];
            }
        }
    }
    grid[m][n]
}
