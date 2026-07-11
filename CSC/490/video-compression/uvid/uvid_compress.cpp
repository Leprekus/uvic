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
   auto YBlockPipeline = Codec::YUVPipeline<Codec::BlockType::YBlock>();
   auto CBlockPipeline = Codec::YUVPipeline<Codec::BlockType::CBlock>();
   // add the width and height into the stream
   output_stream.push_u32(height);
   output_stream.push_u32(width);
   // 8x8 blocks for each frame
   // read Y, Cb, Cr (3 bytes of data) from stdin into the active_frame
   while (reader.read_next_frame()){
      output_stream.push_byte(1); //Use a one byte flag to indicate whether there is a frame here
      YUVFrame420& frame = reader.frame();
      Matrix8d  Yb(8, 8), Cbb(8, 8), Crb(8, 8);
      
      // process Y frame
      for(auto &&y_frame: YBlockPipeline.chunk_frame(width, height, 8)){
         for(auto &&[x, y]: y_frame)
            Yb(x % 8, y % 8) = frame.Y(x, y);
         Yb = YBlockPipeline.DCTForward(Yb);
         for(auto &&[x, y]: y_frame)
            output_stream.push_byte(Yb(x % 8, y % 8));
      }
      for(auto &&c_frame: CBlockPipeline.chunk_frame(width, height, 8)){
         for(auto &&[x, y]: c_frame)
            Cbb(x % 8, y % 8) = frame.Cb(x, y);
         Cbb = CBlockPipeline.DCTForward(Cbb);
         for(auto &&[x, y]: c_frame)
            output_stream.push_byte(Cbb(x % 8, y % 8));
      }
      for(auto &&c_frame: CBlockPipeline.chunk_frame(width, height, 8)){
         for(auto &&[x, y]: c_frame)
            Crb(x % 8, y % 8) = frame.Cr(x, y);
         Crb = CBlockPipeline.DCTForward(Crb);
         for(auto &&[x, y]: c_frame)
            output_stream.push_byte(Crb(x % 8, y % 8));
      }


      //for (u32 y = 0; y < height; y++)
      //   for (u32 x = 0; x < width; x++)
      //      output_stream.push_byte(frame.Y(x,y));
      /*
         for (u32 y = 0; y < height/2; y++)
         for (u32 x = 0; x < width/2; x++)
         output_stream.push_byte(frame.Cb(x,y));
         for (u32 y = 0; y < height/2; y++)
         for (u32 x = 0; x < width/2; x++)
         output_stream.push_byte(frame.Cr(x,y));

*/
   }
  
   output_stream.push_byte(0); //Flag to indicate end of data
   output_stream.flush_to_byte();

   return 0;
}
