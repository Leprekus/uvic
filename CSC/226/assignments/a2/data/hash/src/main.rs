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
            data: vec![-1; capacity]
        }
    }

    fn linear(&self, k: usize) -> usize{
        // murmurhash32_mix32
        let mut k = k;
        k ^= k.wrapping_shr(16);
        k = k.wrapping_rem(0x85ebca6b_usize);
        k ^= k.wrapping_shr(13);
        k = k.wrapping_mul(0xc2b2ae35_usize);
        k ^= k.wrapping_shl(16);
        k
    }
    fn double(&self, k: usize) -> usize {
        
        (10 as usize).wrapping_mul(k)
                   .wrapping_add(6)

    }
    fn h(&self, k: String, i: usize) -> usize{

        let mut hash: usize = 0;
        for byte in k.bytes() {
            hash += byte as usize;
        }
        
        match self.method {
            Method::Linear => 
                ( self.linear(hash).wrapping_add(i) ).wrapping_rem(CAPACITY as usize),

            Method::Double => {
                let h1 = self.double(hash);
                let h2 = self.linear(hash);
                ( h1.wrapping_add( i.wrapping_mul(h2) ) )
                .wrapping_rem(CAPACITY as usize)
            }
        }
    }
    fn insert(&mut self, k: String) {
        if self.size == self.capacity {
            panic!("table full");
        }
        let mut i = 0;
        let mut idx = self.h(k.clone(), i);
        while self.data[idx] != -1 {
            i += 1;
            idx = self.h(k.clone(), i);
        }
        self.data[idx as usize] = self.h(k, i) as i32;
        self.size += 1;
        self.alpha = self.size.wrapping_div(self.capacity);


    }

    fn clear(&mut self) {
        self.data = vec![-1; self.capacity];
        self.size = 0;
    }

}
const CAPACITY: usize = 13;

struct Reader {}
impl Reader {

    fn read_stdin(top: usize, mut callback: impl FnMut(String)) {
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

    
    let mut hash_linear = Hash::new(CAPACITY, Method::Linear);
    let mut hash_double = Hash::new(CAPACITY, Method::Double);
    Reader::read_stdin(100_000, |str| {

        if hash_linear.size == hash_linear.capacity {
            hash_linear.clear();
        } else { 
            println!("clearing");
            hash_linear.insert(str.clone()); 
        }

        if hash_double.size == hash_linear.capacity {
            hash_double.clear();
        } else { 
            hash_double.insert(str); 
        }
    });
}
