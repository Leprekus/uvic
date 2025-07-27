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

fn boundary(u: f64, v: f64, sigma: f64) -> f64 {
    let numerator = (u - v).powi(2);
    let denominator = 2.0 * sigma.powi(2);
    (-numerator/denominator).exp()
}

fn get_neighbors(i: u32, j: u32, width: u32, height: u32, pixels: &Vec<u8>) -> Vec<(usize, f64)> {
    let mut neighbors: Vec<(usize, f64)> = Vec::new();

    let i = i as i32;
    let j = j as i32;
    let width = width as i32;
    let height = height as i32;


    let u = pixels[(i * width + j) as usize] as i32;

    let directions = [
        (-1, -1), (-1, 0), (-1, 1), // top-left, top, top-right
        (0, -1),           (0, 1),  // left,       right
        (1, -1),  (1, 0),  (1, 1),  // bottom-left, bottom, bottom-right
    ];

    for (di, dj) in directions {
        let ni = i + di;
        let nj = j + dj;

        if ni >= 0 && nj >= 0 && ni < height && nj < width {
            let neighbor_idx = (ni * width + nj) as usize;
            let v = pixels[neighbor_idx] as i32;
            neighbors.push((neighbor_idx, boundary(u as f64, v as f64, 30 as f64)));
        }
    }
    neighbors
}
type Graph = Vec<Vec<(usize, f64)>>;
fn create_graph(pixels : &Vec<u8>, width: u32, height: u32) -> Graph{
    
    (0..pixels.len()).map(|i|
        get_neighbors(i as u32 / width as u32, (i as u32 % width) as u32, width, height, pixels)
    ).collect()
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
