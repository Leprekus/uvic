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
#include "utils.hpp"
#include "output_stream.hpp"
#include "yuv_stream.hpp"
#include "dct.hpp"
#include "quantize.hpp"
#include "yuv_pipeline.hpp"

bool printed = false;
auto write_mb_to_stream(OutputBitStream &stream, Macroblock &mb) {
   //std::cerr << "original Y" << std::endl;
   //std::cerr << mb.Y;
   transform_and_quantize<QYBlock>(mb.Y.block<8, 8>(0, 0)); // Top-Left
   transform_and_quantize<QYBlock>(mb.Y.block<8, 8>(0, 8)); // Top-Right
   transform_and_quantize<QYBlock>(mb.Y.block<8, 8>(8, 0)); // Bottom-Left
   transform_and_quantize<QYBlock>(mb.Y.block<8, 8>(8, 8)); // Bottom-Right
                                                      
   transform_and_quantize<QCbBlock>(mb.Cb);
   transform_and_quantize<QCbBlock>(mb.Cr);
   

   for(auto y = 0; y < 16; y++)
      for(auto x = 0; x < 16; x++)
         stream.push_byte(mb.Y(x, y));

   for(auto y = 0; y < 8; y++)
      for(auto x = 0; x < 8; x++)
         stream.push_byte(mb.Cb(x, y));

   for(auto y = 0; y < 8; y++)
      for(auto x = 0; x < 8; x++)
         stream.push_byte(mb.Cr(x, y));
   if(!printed) {
      printed = true;
      std::cerr << "compressor transformed Y" << std::endl;
      std::cerr << mb.Y;
      std::cerr << "compressor reconstructed Y" << std::endl;
      reconstruct_block<QYBlock>(mb.Y.block<8, 8>(0, 0)); // Top-Left
      reconstruct_block<QYBlock>(mb.Y.block<8, 8>(0, 8)); // Top-Right
      reconstruct_block<QYBlock>(mb.Y.block<8, 8>(8, 0)); // Bottom-Left
      reconstruct_block<QYBlock>(mb.Y.block<8, 8>(8, 8)); // Bottom-Right
      std::cerr << mb.Y;
   }

}
int main(int argc, char** argv){

   if (argc < 4){
      std::cout << "Usage: " << argv[0] << " <width> <height> <low/medium/high>" << std::endl;
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
   // 8x8 blocks for each frame
   // read Y, Cb, Cr (3 bytes of data) from stdin into the active_frame
   bool haveIFrame = false;
   bool printed = false;
   Macroblock mb;
   mb.Y.setZero();
   mb.Cb.setZero();
   mb.Cr.setZero();
   std::vector<YUVFrame420>frame_buffer(64, YUVFrame420(width, height));
   while (reader.read_next_frame()){
      YUVFrame420& frame = reader.frame();
      output_stream.push_byte(1); // push flag indicating there is a frame and it's an IFRAME
      for(auto y0 = 0; y0 < height; y0 += 16) {
         for(auto x0 = 0; x0 < width; x0 += 16) {
            /* fill blocks */
            for(auto y = 0; y < 16; y++) {
               for(auto x = 0; x < 16; x++) {
                  /* fill Y block */
                  mb.Y(x, y) = frame.Y(x0 + x, y0 + y);
                  /* fill Cb, Cr blocks */
                  if(x < 8 && y < 8) {
                     mb.Cb(x, y) = frame.Cb(x0/2 + x, y0/2 + y);
                     mb.Cr(x, y) = frame.Cr(x0/2 + x, y0/2 + y);
                  }
               }
            }
            /* create an I-Frame every 64 frames */
            if(false || frame_buffer.size() % 64) {}
            write_mb_to_stream(output_stream, mb);
         }
         
      }
   }
   output_stream.push_byte(0); //Flag to indicate end of data
   output_stream.flush_to_byte();
   return 0;
}
