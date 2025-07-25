use image::{ ImageReader, DynamicImage, ImageError };

fn load_img() -> Result<DynamicImage, ImageError>{
}
fn main() {
    let img = ImageReader::open("../../la-primavera-boticelli.jpg")?.decode()?;
 
    println!("Hello, world!");
}
