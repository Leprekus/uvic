/* uvid_compress.cpp
   CSC 485B/578B - Data Compression - Summer 2023

   Starter code for Assignment 4

   Reads video data from stdin in uncompresed YCbCr (YUV) format 
   (With 4:2:0 subsampling). To produce this format from 
   arbitrary video data in a popular format, use the ffmpeg
   tool and a command like 

   ffmpeg -i videofile.mp4 -f rawvideo -pixel_format yuv420p - 2>/dev/null | ./this_program <width> height>

   Note that since the width/height of each frame is not encoded into the raw
   video stream, those values must be provided to the program as arguments.

   B. Bird - 2023-07-08
   */

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <cassert>
#include <cstdint>
#include <tuple>

#include <ranges>
#include <Eigen/Dense>
#include "output_stream.hpp"
#include "yuv_stream.hpp"
#include "dct.hpp"
#include "quantize.hpp"
#include "yuv_pipeline.hpp"


int main(int argc, char** argv){
   Eigen::Vector2d V;

   if (argc < 4){
      std::cerr << "Usage: " << argv[0] << " <width> <height> <low/medium/high>" << std::endl;
      return 1;
   }
   // convert arguments to uints
   u32 width = std::stoi(argv[1]);
   u32 height = std::stoi(argv[2]);
   std::string quality{argv[3]};

   // create instances of reader & output stream
   YUVStreamReader reader {std::cin, width, height};
   OutputBitStream output_stream {std::cout};

   // add the width and height into the stream
   output_stream.push_u32(height);
   output_stream.push_u32(width);
   assert(width == 720 && height == 480);

   // read Y, Cb, Cr (3 bytes of data) from stdin into the active_frame
   while (reader.read_next_frame()){
      output_stream.push_byte(1); //Use a one byte flag to indicate whether there is a frame here
      YUVFrame420& frame = reader.frame();
      // process Y subsampling
      //for (u32 y = 0; y < height; y++)
      //   for (u32 x = 0; x < width; x++)
      //      output_stream.push_byte(frame.Y(x,y));

      // number of 8x8 blocks in a frame 
      //auto blocks_y = std::views::iota(0U, height) | std::views::chunk(8); 
      //auto blocks_x = std::views::iota(0U, width)  | std::views::chunk(8); 
      //auto blocks = std::views::cartesian_product(blocks_x, blocks_y);
      // Y, Cb, & Cr 8x8 blocks
      Eigen::MatrixXd Yb(8, 8), Cbb(8,8), Crb(8,8);
      //for(auto &&[block_x, block_y]: blocks) {
      //   for(int x: block_x) {
      //      for(int y: block_y) // populate the 8x8 matrix
      //         M(x % 8, y % 8) = frame.Y(x, y);
      //   } 
      //   M = Codec::DCT::forward(M); // apply the DCT transformation
      //   M = Codec::Quantize::forward(M);
      //   for(int x: block_x) {
      //      for(int y : block_y)  // push the DCT matrix into the stream
      //         output_stream.push_byte(M(x % 8, y % 8));
      //   }
      //}
      
      for(auto Yframe_view: Codec::YUVPipeline::chunk_frame(width, height)) {
         for(auto &&[x, y]: Yframe_view)  
            Yb(x % 8, y % 8) = frame.Y(x, y);

         for(auto &&[x, y]: Yframe_view)
            output_stream.push_byte(Yb(x % 8, y % 8));
      }
         
      // NOTE: if Cb & Cr blocks get mixed separate back each into its own loop
      for(auto Cframes_view: Codec::YUVPipeline::chunk_frame(width/2, height/2)) {
         for(auto &&[x, y]: Cframes_view) {
            Cbb(x % 8, y % 8) = frame.Cb(x, y);
            Crb(x % 8, y % 8) = frame.Cr(x, y);
         }  

         for(auto &&[Yx, Yy]: Cframes_view) {
            output_stream.push_byte(Cbb(Yx % 8, Yy % 8));
            output_stream.push_byte(Crb(Yx % 8, Yy % 8));
         }
      }
      //Codec::YUVPipeline::group_by(width/2, height/2);
      //Codec::YUVPipeline::group_by(width/2, height/2);

      // process Cb subsampling
      //for (u32 y = 0; y < height/2; y++)
      //   for (u32 x = 0; x < width/2; x++)
      //      output_stream.push_byte(frame.Cb(x,y));
      // process Cr subsampling
      //for (u32 y = 0; y < height/2; y++)
      //   for (u32 x = 0; x < width/2; x++)
      //      output_stream.push_byte(frame.Cr(x,y));
   }

   output_stream.push_byte(0); //Flag to indicate end of data
   output_stream.flush_to_byte();

   return 0;
}
