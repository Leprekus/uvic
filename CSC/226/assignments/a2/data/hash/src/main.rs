use std::io::BufRead;
enum Method {
    Linear,
    Double
}
struct Hash {
    capacity: usize,
    method: Method,
    size: usize,
    alpha: usize,
    data: Vec<i32>,
}


impl Hash {
    pub fn new(capacity: usize, method: Method) -> Self {
        Self{
            method,
            capacity,
            size: 0,
            alpha: 0,
            data: Vec::with_capacity(capacity)
        }
    }

    fn linear(self, k: i32) -> i32 {
        0
    }
    fn double(self, k: i32) -> i32{
        0
    }
    fn h(self, k: i32) -> i32{
        match self.method {
            Method::Linear => self.linear(k),
            Method::Double => self.double(k)
        }
    }

}
const CAPACITY: usize = 13;

struct Reader {}
impl Reader {

    fn read_stdin(top: usize, callback: impl Fn(String)) {
        let stdin = std::io::stdin();
        for (i, line) in stdin.lock().lines().enumerate() {
            match line {
                Ok(data) => callback(data),
                Err(e) => panic!("should not be here! {}", e)
            }
            if i >= top { break }

        }
    }
}
fn main(){

    let n = 100_000;
    Reader::read_stdin(n, |_| {});
    
    let hash_linear = Hash::new(CAPACITY, Method::Linear);
    //let hash_double = Hash::new(CAPACITY, Method::Double);
}
