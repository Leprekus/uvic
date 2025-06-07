use std::io::BufRead;
use std::fs::OpenOptions;
use std::io::{Write};
#[derive(Debug)]
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
    records: Vec<String>
}


impl Hash {
    pub fn new(capacity: usize, method: Method) -> Self {
        let headers = String::from("item number, key, status, load factor");
        let mut records = Vec::new();
        records.push(String::from(format!("{:#?},,,", method)));
        records.push(headers);
        Self{
            method,
            capacity,
            size: 0,
            alpha: 0,
            data: vec![-1; capacity],
            records, 
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
            //if we push here it was a miss
            self.records.push(
                String::from(
                    format!("{}, {}, {}, {}", self.size, idx, "miss", (self.size as f64)/(self.capacity as f64))
                )
            );
            i += 1;
            idx = self.h(k.clone(), i);
        }
        self.records.push(
                String::from(
                    format!("{}, {}, {}, {}", self.size, idx, "hit", (self.size as f64)/(self.capacity as f64))
                )
            );

        self.data[idx as usize] = self.h(k, i) as i32;
        self.size += 1;
        self.alpha = self.size.wrapping_div(self.capacity);


    }

    fn clear(&mut self) {
        self.data = vec![-1; self.capacity];
        self.size = 0;
        self.records = Vec::new();
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

struct Writer {}
impl Writer {
    //https://stackoverflow.com/questions/31192956/whats-the-de-facto-way-of-reading-and-writing-files-in-rust-1-x
    fn write(path: String, data: Vec<String>) {
        //println!("records\n{:?}", data);
        let data: Vec<u8> = data
                .iter()
                .flat_map(|s| s.as_bytes().iter().copied().chain(std::iter::once(b'\n')))
                .collect();
        //std::fs::write(path, data).expect("Unable to write file")
        let mut file = OpenOptions::new()
            .create(true)      // create if it doesn't exist
            .append(true)      // append if it does
            .open(path)
            .expect("unable to open / create file");

        file.write_all(&data).expect("unable to write to file");
    
    }
}
fn main(){

    
    let mut hash_linear = Hash::new(CAPACITY, Method::Linear);
    let mut hash_double = Hash::new(CAPACITY, Method::Double);
    Reader::read_stdin(100_000, |str| {

        if hash_linear.size == hash_linear.capacity {
            Writer::write(String::from("../records_linear.csv"), hash_linear.records.clone());
            hash_linear.clear();
        } else { 
            hash_linear.insert(str.clone()); 
        }

        if hash_double.size == hash_linear.capacity {
            Writer::write(String::from("../records_double.csv"), hash_linear.records.clone());
            hash_double.clear();
        } else { 
            hash_double.insert(str); 
        }
    });
}
