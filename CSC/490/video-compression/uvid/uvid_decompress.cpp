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
#include "utils.hpp"

bool printed = false;
std::optional<FrameBuffer> frame_buffer;
void write_mb(YUVFrame420 &frame, Macroblock &mb, auto x0, auto y0) {
   for(auto y = 0; y < 16; y++)
      for(auto x = 0; x < 16; x++)
         frame.Y(x0 + x, y0 + y) = mb.Y(x, y);

   for(auto y = 0; y < 8; y++)
      for(auto x = 0; x < 8; x++)
         frame.Cb(x0/2 + x, y0/2 + y) = mb.Cb(x, y);

   for(auto y = 0; y < 8; y++)
      for(auto x = 0; x < 8; x++)
         frame.Cr(x0/2 + x, y0/2 + y) = mb.Cr(x, y);
}
auto reconstruct_mb(InputBitStream &stream, YUVFrame420 &frame, Macroblock &mb, auto x0, auto y0) {
   reconstruct_block<QYBlock>(mb.Y.block<8, 8>(0, 0)); // Top-Left
   reconstruct_block<QYBlock>(mb.Y.block<8, 8>(0, 8)); // Top-Right
   reconstruct_block<QYBlock>(mb.Y.block<8, 8>(8, 0)); // Bottom-Left
   reconstruct_block<QYBlock>(mb.Y.block<8, 8>(8, 8)); // Bottom-Right
                                                      
   reconstruct_block<QCbBlock>(mb.Cb);
   reconstruct_block<QCbBlock>(mb.Cr);
   frame_buffer->push_mb(mb); // store decompressed I-frame
   //if(!printed){
   //   printed = true;
   //   std::cerr << "decompressor reconstructed Y" << std::endl;
   //   std::cerr << mb.Y;
   //}

   write_mb(frame, mb, x0, y0); 
}

int count = 0;
void reconstruct_vector(YUVFrame420 &frame, Macroblock &mb, auto x, auto y) {
      if(count == 105 && !printed) {
      std::cerr << "DECOMPRESSOR: delta Y" << std::endl;
      std::cerr << mb.Y << std::endl;
   }
   
   Macroblock &decompressed_mb = frame_buffer->get_mb(x, y);  
   // add dequantize and add delta
   mb.Y  *= 2;
   mb.Cr *= 2;
   mb.Cb *= 2;

   mb.Y  +=  decompressed_mb.Y;
   mb.Cr += decompressed_mb.Cr;
   mb.Cb += decompressed_mb.Cb;


   write_mb(frame, mb, x, y); 
   if(count == 105 && !printed) {
      printed = true;
      //std::cerr << "CACHED: decompressed Y" << std::endl;
      //std::cerr << decompressed_mb.Y << std::endl;
      std::cerr << "RECONSTRUCTED:  Y" << std::endl;
      std::cerr << mb.Y << std::endl;
   }
}
int main(int argc, char** argv){

   //Note: This program must not take any command line arguments. (Anything
   //      it needs to know about the data must be encoded into the bitstream)

   InputBitStream input_stream {std::cin};

   u32 height {input_stream.read_u32()};
   u32 width {input_stream.read_u32()};
   // 8x8 blocks for each frame
   YUVStreamWriter writer {std::cout, width, height};
   Macroblock mb;
   mb.Y.setZero();
   mb.Cb.setZero();
   mb.Cr.setZero();
   frame_buffer = FrameBuffer{ width, height };

   while (input_stream.read_byte()){
      count++;
      YUVFrame420& frame = writer.frame();
      writer.write_frame();
      for(auto y0 = 0; y0 < height; y0 += 16) {
         for(auto x0 = 0; x0 < width; x0 += 16) {
            /* fill Y */
            for(auto y = 0; y < 16; y++)
               for(auto x = 0; x < 16; x++)
                  mb.Y(x, y) = static_cast<char>(input_stream.read_byte());
            /* fill Cb */
            for(auto y = 0; y < 8; y++)
               for(auto x = 0; x < 8; x++)
                   mb.Cb(x, y) = static_cast<char>(input_stream.read_byte());
            /* fill Cr */
            for(auto y = 0; y < 8; y++)
               for(auto x = 0; x < 8; x++)
                  mb.Cr(x, y) = static_cast<char>(input_stream.read_byte());
            /* create an I-Frame every 64 frames */
            if(frame_buffer->frame_count() == 0)
               reconstruct_mb(input_stream, frame, mb, x0, y0);
            else 
               reconstruct_vector(frame, mb, x0, y0);
         }
         
      }
   }
   //exit(1);
   return 0;
}
