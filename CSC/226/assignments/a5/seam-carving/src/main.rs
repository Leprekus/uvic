use std::path::{ self, Path, PathBuf };
use std::fs;
use image::{ ImageReader };

fn main() {
    let mut path = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    path.pop();
    path = path.join("la-primavera-boticelli.jpg");
    let img = ImageReader::open(path.as_path())
       .unwrap().decode().unwrap();
    println!("{:?}", img);
}
