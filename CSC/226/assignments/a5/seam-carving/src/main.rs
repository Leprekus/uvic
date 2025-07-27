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

fn boundary(v: u32, neighbors: Vec<i32>) -> i32 {
    1
}
fn get_neighbors(u: u32, v: u32, width: u32, height: u32, pixels: &Vec<u8>) -> Vec<i32> {
    let mut neighbors: Vec<i32> = Vec::new();

    let u = u as i32;
    let v = v as i32;
    let width = width as i32;
    let height = height as i32;

    let directions = [
        (-1, -1), (-1, 0), (-1, 1), // top-left, top, top-right
        (0, -1),           (0, 1),  // left,       right
        (1, -1),  (1, 0),  (1, 1),  // bottom-left, bottom, bottom-right
    ];

    for (du, dv) in directions {
        let nu = u + du;
        let nv = v + dv;

        if nu >= 0 && nv >= 0 && nu < height && nv < width {
            let n_idx = (nu * width + nv) as usize;
            neighbors.push(pixels[n_idx] as i32);
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
    for u in 0..height {
        for v in 0..width {
            let neighbors = get_neighbors(u, v, width, height, pixels);
            let w = boundary(pixels[(u * width + v) as usize] as u32, neighbors);
            g[u as usize][v as usize] = w; 
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
