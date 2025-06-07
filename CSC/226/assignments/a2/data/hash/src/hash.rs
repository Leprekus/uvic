struct Hash {
    size: usize,
    capacity: usize,
    load_factor: usize,
    data: Vec<i32>
}

impl Hash {
    pub fn new(capacity: usize) -> Self {
        Self{ 
            size: 0, 
            capacity, 
            load_factor: 0, 
            data: Vec::with_capacity(capacity) 
        }
    }
}
