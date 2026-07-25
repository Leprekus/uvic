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
#include <bitset>
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

/* pass pixel coordinates */
void reconstruct_vector(YUVFrame420 &frame, Macroblock &mb, int x0, int y0) {
   //TODO: pick specific frame to reconstruct from based on the current B-frame's index
   
   auto [x, y, idx] = mb.vect;
   assert(frame_buffer->frame_count() >= 1);
   assert(idx <= 3);
   if(mb.is_copy){ // process copied p-frame
      Macroblock copy_dec = frame_buffer->get_frame_mb(idx, x + x0, y + y0);
      copy_dec.is_copy = true;
      // push blocks into the stream
      frame_buffer->push_mb(copy_dec);

   } else { // process delta p-frame
      Macroblock &decompressed_mb = frame_buffer->get_frame_mb(idx, x, y);  
      //Macroblock &decompressed_mb = frame_buffer->get_frame_mb(z, x, y);  
      predicted_inverse(mb, decompressed_mb); 
      frame_buffer->push_mb(mb);
   }
   
}
void decode_mb(YUVFrame420 &frame, Macroblock &mb,
      auto x0, auto y0) {

   bool is_first_frame = frame_buffer->frame_count() < 1;
   // decode an I-frame
   if(is_first_frame) {
      assert(!mb.is_copy);
      intra_reconstruct(frame_buffer, qual, mb, frame_buffer->frame_count(), x0, y0, mb.vect);
      frame_buffer->push_mb(mb); // store decompressed I-frame
   } else { // decode a P-frame
      reconstruct_vector(frame, mb, x0, y0);
   
   }

}

int M[4] = {0};
BlockVect read_vector(InputBitStream &stream, Macroblock &mb) {
   u8 copy = static_cast<u8>(stream.read_byte());
   if(copy == 1U)
      mb.is_copy = true;
   else 
      mb.is_copy = false;

   
   i16 x = static_cast<i16>(
      static_cast<u8>(stream.read_byte())<<8 |
      static_cast<u8>(stream.read_byte())
   );

   i16 y = static_cast<i16>(
      static_cast<u8>(stream.read_byte())<<8 |
      static_cast<u8>(stream.read_byte())
   );

   char z = static_cast<i8>(stream.read_byte()); 

   //if(!M[frame_buffer->frame_count()]) {
   //   M[frame_buffer->frame_count()] = 1;
   //   std::cerr << " frame " << frame_buffer->frame_count() << " copy " << mb.is_copy << " x " << x << " y " << y << " z " << static_cast<int>(z) << "\n"; 
   //}
   return BlockVect(x, y, z); 
}


// TODO: create all frames off of the first one in the buffer to make sure it is working
//void reconstruct_buffered(YUVFrame420 &frame, int width) {
//   for(int i: frame_decoding_order) {
//
//      //std::cerr << "decoding B " << i << "\n";
//      int x0 = 0;
//      int y0 = 0;
//      for(Macroblock &mb: frame_buffer->get_frame(i)) {
//         // reconstruct everything from the first frame for now..
//         reconstruct_vector(frame, mb, 0, 0, 0);
//         x0  += 16;
//         if(x0 >= width) {
//            x0  = 0;
//            y0 += 16;
//         }
//
//      }
//   }
//}
int bread = 0;
void write_frames_to_stream(YUVStreamWriter &writer, int width) {
   assert(frame_buffer->frame_count() <= 4);
   YUVFrame420 &frame = writer.frame();
   for(int i = 0; i < frame_buffer->frame_count(); i++) {
      int x0 = 0;
      int y0 = 0;
      bool printed = false;
      for(const Macroblock &mb: frame_buffer->get_frame(i)){
      if(!printed) {
            printed = true;
            //std::cerr << " frame " << i << " copied " << mb.is_copy << "\n";
         }
         bread++;
         write_mb(frame, mb, x0, y0);
         x0  += 16;
         if(x0 >= width) {
            x0  = 0;
            y0 += 16;
         }
      }
      writer.write_frame();

   }
   
}
void read_into_mb(InputBitStream &input_stream, Macroblock &mb) {
   
   BlockVect vect = read_vector(input_stream, mb);
   mb.vect = vect;
   if(mb.is_copy) return;
   bitstream_to_compressed_mb(mb, input_stream);
   //std::cerr << "CY\n" <<mb.Y<<"\n";
   //std::cerr << "Cb\n" <<mb.Cb<<"\n";
   //std::cerr << "Cr\n" <<mb.Cr<<"\n";
   //exit(1);
   ///
   /* fill Y */
   //for(int y = 0; y < 16; y++)
   //   for(int x = 0; x < 16; x++)
   //      mb.Y(x, y) = static_cast<i8>(input_stream.read_byte());
   ///* fill Cb */
   //for(int y = 0; y < 8; y++)
   //   for(int x = 0; x < 8; x++)
   //      mb.Cb(x, y) = static_cast<i8>(input_stream.read_byte());
   ///* fill Cr */
   //for(int y = 0; y < 8; y++)
   //   for(int x = 0; x < 8; x++)
   //      mb.Cr(x, y) = static_cast<i8>(input_stream.read_byte());
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
   YUVFrame420& frame = writer.frame();
   int count = 0;
   while (input_stream.read_byte()){
      
      //writer.write_frame(); 
      for(auto y0 = 0; y0 < height; y0 += 16) {
         for(auto x0 = 0; x0 < width; x0 += 16) {
            count++; 
            read_into_mb(input_stream, mb);
            //if(count == 396 || count == 3960) {
            //   std::cerr << "Y\n"<<mb.Y << "\n"<<"Cb"<<mb.Cb<<"\n"<<"Cr"<<mb.Cr<<"\n";
            //}

            bool buffer_is_full = frame_buffer->frame_count() >= 4;
            if(buffer_is_full) 
               throw std::runtime_error("buf_compressed overflow");
            decode_mb(frame, mb, x0, y0);

         }
      }
      input_stream.flush_to_byte();

      bool buffer_is_full = frame_buffer->frame_count() >= 4;
      if(buffer_is_full)  {
         write_frames_to_stream(writer, width);
         frame_buffer->clear();
         std::memset(M, 0, sizeof(M));
      }
      
   }
   if(frame_buffer->size()) write_frames_to_stream(writer, width);
   std::cerr << "read " << bread << "\n";
   return 0;
}
