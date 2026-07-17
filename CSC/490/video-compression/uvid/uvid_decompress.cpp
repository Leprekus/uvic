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
Quality qual = Quality::MED;
void write_mb(YUVFrame420 &frame, const Macroblock &mb, auto x0, auto y0) {
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
void reconstruct_mb( YUVFrame420 &frame, Macroblock &mb,
      auto x0, auto y0, std::pair<i8, i8> offset) {

   intra_reconstruct(frame_buffer, qual, mb, 0, 0, 0, offset);

   frame_buffer->push_mb(mb); // store decompressed I-frame
}

/* pass pixel coordinates */
void reconstruct_vector(YUVFrame420 &frame, Macroblock &mb, int i, int x, int y) {
   assert(x >= 0 && y >=0); 
   //TODO: pick specific frame to reconstruct from based on the current B-frame's index
   //Macroblock &decompressed_mb = frame_buffer->get_mb(x, y);  
   //if(!printed) {
   //   print(mb, "compressor bframe 1 in");
   //   printed = true;
   //}

   
   Macroblock &decompressed_mb = frame_buffer->get_frame_mb(i, x, y);  
   predicted_inverse(mb, decompressed_mb); 
   //NEW
   //frame_buffer->push_mb(mb);
   
}
std::pair<i8, i8> read_vector(InputBitStream &stream) {
   char x = static_cast<i8>(stream.read_byte()); 
   if(x == -1)
      return std::pair(-1, -1);
   char y = static_cast<i8>(stream.read_byte()); 
   return std::pair(x, y); 
}

void collect_mb(std::optional<FrameBuffer> &buf_compressed, Macroblock &mb, int x0, int y0) {
    //if(is_p_or_iframe)
    //   reconstruct_mb(input_stream, frame, mb, x0, y0, vect);
    //else if (is_between_first_and_last_frame) {
    //   reconstruct_vector(frame, mb, x0, y0);
    //}
    frame_buffer->push_mb(mb);

}

// TODO: create all frames off of the first one in the buffer to make sure it is working
void reconstruct_buffered(YUVFrame420 &frame, int width) {
   for(int i: frame_decoding_order) {

      //std::cerr << "decoding B " << i << "\n";
      int x0 = 0;
      int y0 = 0;
      for(Macroblock &mb: frame_buffer->get_frame(i)) {
         // reconstruct everything from the first frame for now..
         reconstruct_vector(frame, mb, 0, 0, 0);
         x0  += 16;
         if(x0 >= width) {
            x0  = 0;
            y0 += 16;
         }

      }
   }
}
int count = 0;
int bread = 0;
int cframe = 0; 
void play_buffer(YUVFrame420 &frame, int width) {
   for(int i: frame_play_order) {
      int x0 = 0;
      int y0 = 0;

      //uncomment to see decoding ordder
      //std::cerr << "playing frame " << idx << "\n";
      cframe++;
      for(const Macroblock &mb: frame_buffer->get_frame(i)){
         bread++;
         write_mb(frame, mb, x0, y0);
         x0  += 16;
         if(x0 >= width) {
            x0  = 0;
            y0 += 16;
         }
         count++;
         int idx = frame_encoding_order[i];

         if(cframe - 1 == 10 && count == 200) { 
            auto _mb = mb;
            std::cerr <<"frame # " << cframe - 1 << " idx " << i << "\n";
            print(mb, "decompressor\n"); 
            //exit(1); 

         }
        
      }
      count = 0;
   }
}
int main(int argc, char** argv){

   //Note: This program must not take any command line arguments. (Anything
   //      it needs to know about the data must be encoded into the bitstream)

   InputBitStream input_stream {std::cin};

   u32 height {input_stream.read_u32()};
   u32 width {input_stream.read_u32()};
   u8 qual_opt = input_stream.read_byte();
   if(qual_opt == Quality::LOW) qual = Quality::LOW;
   else if(qual_opt == Quality::MED) qual = Quality::MED;
   else if(qual_opt == Quality::HIGH) qual = Quality::HIGH;
   else {
      std::cerr << "invalid quality: " << static_cast<int>(qual_opt); 
      exit(1);
   };
   // 8x8 blocks for each frame
   YUVStreamWriter writer {std::cout, width, height};
   Macroblock mb;
   mb.Y.setZero();
   mb.Cb.setZero();
   mb.Cr.setZero();
   frame_buffer = FrameBuffer{ width, height };
   while (input_stream.read_byte()){
      YUVFrame420& frame = writer.frame();
      
      writer.write_frame(); 
      for(auto y0 = 0; y0 < height; y0 += 16) {
         for(auto x0 = 0; x0 < width; x0 += 16) {
            std::pair<i8, i8> vect = read_vector(input_stream);
	         mb.vect = vect;
            /* fill Y */
            for(int y = 0; y < 16; y++)
               for(int x = 0; x < 16; x++)
                  mb.Y(x, y) = static_cast<i8>(input_stream.read_byte());
            /* fill Cb */
            for(int y = 0; y < 8; y++)
               for(int x = 0; x < 8; x++)
                   mb.Cb(x, y) = static_cast<i8>(input_stream.read_byte());
            /* fill Cr */
            for(int y = 0; y < 8; y++)
               for(int x = 0; x < 8; x++)
                  mb.Cr(x, y) = static_cast<i8>(input_stream.read_byte());
            int frame_count = frame_buffer->frame_idx();
            bool is_p_or_iframe = is_ip_frame[frame_count];
            bool is_not_full = 0 < frame_count && frame_count < 16;
            if(is_p_or_iframe) {
               reconstruct_mb(frame, mb, x0, y0, vect);
            } else if (!is_p_or_iframe) {
		assert(frame_buffer->size() >= 396 * 2);
               //reconstruct_vector(frame, mb, x0, y0);
               collect_mb(frame_buffer, mb, x0, y0);
            }
         }
      }
      
      bool frame_buffer_is_full = frame_buffer->frame_count() - 1 == 17;
      if(frame_buffer_is_full) {
         reconstruct_buffered(frame, width);
         play_buffer(frame, width);
         frame_buffer->clear();
      }
   }
   std::cerr << " read " << bread << "\n";

   //print(frame_buffer->get_frame_mb(0, 0, 0), "decompressor frame 0, block 0");
   return 0;
}
