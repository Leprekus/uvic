/* uvid_decompress.cpp
   CSC 485B/578B - Data Compression - Summer 2023

   Starter code for Assignment 4

   This placeholder code reads the (basically uncompressed) data produced by
   the uvid_compress starter code and outputs it in the uncompressed 
   YCbCr (YUV) format used for the sample video input files. To play the 
   the decompressed data stream directly, you can pipe the output of this
   program to the ffplay program, with a command like 

   ffplay -f rawvideo -pixel_format yuv420p -framerate 30 -video_size 352x288 - 2>/dev/null
   (where the resolution is explicitly given as an argument to ffplay).

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
#include <algorithm>
#include <Eigen/Dense>
#include "input_stream.hpp"
#include "yuv_stream.hpp"
#include "dct.hpp"
#include "quantize.hpp"
#include "yuv_pipeline.hpp"


int main(int argc, char** argv){

   //Note: This program must not take any command line arguments. (Anything
   //      it needs to know about the data must be encoded into the bitstream)

   InputBitStream input_stream {std::cin};

   u32 height {input_stream.read_u32()};
   u32 width {input_stream.read_u32()};
   // 8x8 blocks for each frame
   YUVStreamWriter writer {std::cout, width, height};
   auto YBlockPipeline = Codec::YUVPipeline<Codec::BlockType::YBlock>();
   auto CBlockPipeline = Codec::YUVPipeline<Codec::BlockType::CBlock>();
   while (input_stream.read_byte()){
      YUVFrame420& frame = writer.frame();
      Matrix8d  Yb(8, 8), Cb(8, 8), Cr(8, 8);
      
      for(auto &&y_frame: YBlockPipeline.chunk_frame(width, height, 8)){
         for(auto &&[x, y]: y_frame)
            Yb(x % 8, y % 8) = static_cast<char>(input_stream.read_byte());
      
         Yb = YBlockPipeline.DCTInverse(Yb);
         for(auto &&[x, y]: y_frame)
            frame.Y(x, y) = Yb(x % 8, y% 8);
      }
      for(auto &&y_frame: CBlockPipeline.chunk_frame(width, height, 8)){
         for(auto &&[x, y]: y_frame)
            Cb(x % 8, y % 8) = static_cast<char>(input_stream.read_byte());
         Cb = CBlockPipeline.DCTInverse(Cb);
         for(auto &&[x, y]: y_frame)
            frame.Cb(x, y) = Cb(x % 8, y % 8);
      }      
      for(auto &&y_frame: CBlockPipeline.chunk_frame(width, height, 8)){
         for(auto &&[x, y]: y_frame)
            Cr(x % 8, y % 8) = static_cast<char>(input_stream.read_byte());
         Cr = CBlockPipeline.DCTInverse(Cr);
         for(auto &&[x, y]: y_frame)
            frame.Cr(x, y) = Cr(x % 8, y % 8);
      }

      //for (u32 y = 0; y < height; y++) 
      //   for (u32 x = 0; x < width; x++)
      //      frame.Y(x,y) = input_stream.read_byte();
      /*
         for (u32 y = 0; y < height/2; y++)
         for (u32 x = 0; x < width/2; x++)
         frame.Cb(x,y) = input_stream.read_byte();
         for (u32 y = 0; y < height/2; y++)
         for (u32 x = 0; x < width/2; x++)
         frame.Cr(x,y) = input_stream.read_byte();
         */
      writer.write_frame();
   }
   return 0;
}
