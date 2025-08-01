use std::iter;
use std::path::{ self, Path, PathBuf };
use std::io::Cursor;
use std::time::SystemTime;
use image::{ GenericImageView, ImageReader };
use kmeans_colors::{ get_kmeans, Kmeans, Sort };
use palette::cast::from_component_slice;
use palette::{IntoColor, Lab, Srgb};
use rand::Rng;
use statrs::distribution::{ Normal, Continuous };

fn get_sd(values: &Vec<f32>) -> f64 {
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

fn fit_normal(data: &Vec<f32>) -> Normal{
    let mean = data.iter().copied().sum::<f32>() as f64 / data.len() as f64;
    let std = get_sd(data);
    Normal::new(mean, std).unwrap()

}
type Graph = Vec<Vec<(usize, f64)>>;
fn create_graph(pixels : &Vec<u8>, width: u32, height: u32) -> Graph{
    
    let seed = rand::rng().random::<u64>();
    let rgb_pixels = (0..pixels.len()).flat_map(|i| {
        let x = pixels[i];
        [x, x, x].into_iter()
    }).collect::<Vec<u8>>();
    let lab : Vec<Lab> = from_component_slice::<Srgb<u8>>(&rgb_pixels)
        .iter()
        .map(|x| x.into_linear().into_color())
        .collect();
    let mut result = Kmeans::new();
    for i in 0..10 {
        let run_result = get_kmeans(
                2,
                50,
                1e-4,
                false,
                &lab,
                seed + i
            );
        if run_result.score < result.score {
            result = run_result;
        }
    }
    let mut cluster_0 = Vec::new();
    let mut cluster_1 = Vec::new();
    result.indices.iter().enumerate().for_each(|(i, &label)| {
        match label {
            0 => cluster_0.push(lab[i].l),
            1 => cluster_1.push(lab[i].l),
            _ => unreachable!()
        };
    });
    let g0 = fit_normal(&cluster_0);
    let g1 = fit_normal(&cluster_1);
    (0..pixels.len()).map(|i|
        get_neighbors(i as u32 / width as u32, (i as u32 % width) as u32, width, height, pixels)
    )
    .collect()
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
    let _ = img.save(dest_path.as_path());
}
