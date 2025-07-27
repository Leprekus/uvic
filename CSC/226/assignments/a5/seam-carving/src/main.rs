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

fn get_neighbors(i: u32, j: u32, width: u32, height: u32, pixels: &Vec<u8>) -> Vec<i32> {
    let mut neighbors: Vec<i32> = Vec::new();

    let i = i as i32;
    let j = j as i32;
    let width = width as i32;
    let height = height as i32;


    let u = (i * width + j) as usize;

    let directions = [
        (-1, -1), (-1, 0), (-1, 1), // top-left, top, top-right
        (0, -1),           (0, 1),  // left,       right
        (1, -1),  (1, 0),  (1, 1),  // bottom-left, bottom, bottom-right
    ];

    for (di, dj) in directions {
        let ni = i + di;
        let nj = j + dj;

        if ni >= 0 && nj >= 0 && nu < height && nj < width {
            let v = pixels[(ni * width + nj) as usize];
            neighbors.push(boundary());
        }
    }
    neighbors
}
type Graph = Vec<Vec<i32>>;
fn create_graph(pixels : &Vec<u8>, width: u32, height: u32) -> Graph{
    
    let mut g: Vec<Vec<i32>> = pixels
        .chunks_exact(width as usize)
        .map(|row| row.iter().map(|&p| p as i32).collect())
        .collect();
    for i in 0..height {
        for j in 0..width {
            let idx = (i * width + j) as usize;
            g[idx] = get_neighbors(i as u32, j as u32, width, height, pixels);
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
