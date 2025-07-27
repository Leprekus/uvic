use std::path::{ self, Path, PathBuf };
use std::io::Cursor;
use image::{ GenericImageView, ImageReader };

fn get_sd(values: &Vec<u8>) -> f64 {
    let (sum, count) = values.iter()
        .fold((0u32, 0u32), |(sum, count), &v| (sum + v as u32, count + 1) );
    let mean = sum as f64 / count as f64;
    let sd: f64 = (
            values.iter().fold( 0f64, | sum, &xi| 
                sum + (xi as f64 - mean).powi(2)
            ) 
            / (count as f64 - 1.0)
        ).sqrt();
    sd
}

fn boundary() -> i32 {
    1
}
type Graph = Vec<Vec<i32>>;
fn create_graph(pixels : &Vec<u8>, width: u32, height: u32) -> Graph{
    let mut g: Graph = vec![ vec![0; width as usize]; height as usize];
    for u in 0..height {
        for v in 0..width {
            g[u as usize][v as usize] = boundary();
        }
    }
    g
}
fn main() {
    let mut path = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    path.pop();

    let src_path = path.join("la-primavera-boticelli.jpg");
    let dest_path = path.join("output.jpg");

    let img = ImageReader::open(src_path.as_path())
       .unwrap().decode().unwrap();
    let img = img.to_luma8();
    let pixels: Vec<u8> = img.pixels().map(|p| p[0]).collect();
    let g = create_graph(&pixels, img.width(), img.height());
    let sd = get_sd(&pixels); 
    let _ = img.save(dest_path.as_path());
    println!("sd {:?}", sd);
}
