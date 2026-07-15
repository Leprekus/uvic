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

#include <cmath>
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
std::optional<FrameBuffer> buf_compressed;
std::optional<FrameBuffer> buf_decompressed;
Quality qual = Quality::MED;
void write_intra_vector(OutputBitStream &stream, std::pair<i8, i8> mb_vector) {
   auto [x, y] = mb_vector;
   if(x == -1 && y == -1)
      return stream.push_byte(x);
   stream.push_byte(x);
   stream.push_byte(y);
}
auto write_mb(OutputBitStream &stream, const Macroblock &mb, std::pair<i8, i8> vect) {

   write_intra_vector(stream, vect);

   for(auto y = 0; y < 16; y++)
      for(auto x = 0; x < 16; x++)
         stream.push_byte(static_cast<i8>(mb.Y(x, y)));

   for(auto y = 0; y < 8; y++)
      for(auto x = 0; x < 8; x++)
         stream.push_byte(static_cast<i8>(mb.Cb(x, y)));

   for(auto y = 0; y < 8; y++)
      for(auto x = 0; x < 8; x++)
         stream.push_byte(static_cast<i8>(mb.Cr(x, y)));
}


Macroblock tmp{
   .Y  = Matrix16d::Zero(16, 16), 
   .Cb = Matrix8d::Zero(8, 8), 
   .Cr = Matrix8d::Zero(8, 8)
};
int get_aad(Macroblock &want, Macroblock &have){
      return (
         ((want.Y  - have.Y).array().abs().sum() +
          (want.Cb - have.Cb).array().abs().sum() +
          (want.Cr - have.Cr).array().abs().sum()) 
         / 384
         );
      
}
std::pair<i8, i8> intra_prediction(Macroblock &mb, auto x, auto y) {
   
   tmp = mb;
   if(x >= 16) {
      Macroblock &left = buf_decompressed->get_mb(x - 16, y); 
      if(get_aad(tmp, left) <= 5) {
         predicted_forward(mb, left);
         return std::pair(-16, 0);
      }
   }
   if(y >= 16) {
      Macroblock &above = buf_decompressed->get_mb(x, y - 16); 
      if(get_aad(tmp, above) <= 5) {
         predicted_forward(mb, above);
         return std::pair(0, - 16);
      }
   }
   if(x >= 16 && y >= 16) {
      Macroblock &topleft = buf_decompressed->get_mb(x - 16, y - 16);
      if(get_aad(tmp, topleft) <= 5) {
         predicted_forward(mb, topleft);
         return std::pair(- 16, - 16);
      }
   }
   iframe_forward(qual, mb);
   return std::pair(-1, -1);
}
auto encode_and_write_mb(OutputBitStream &stream, Macroblock &mb, auto x, auto y) {
   auto vect = intra_prediction(mb, x, y); 
   //write_mb(stream, mb, vect);
   mb.vect = vect;
   buf_compressed->push_mb(mb);
   intra_reconstruct(buf_decompressed, qual, mb, x, y, vect);  
   buf_decompressed->push_mb(mb);
}

int count = 0;
void encode_and_write_vector_search(OutputBitStream &stream, Macroblock &mb, int x, int y) {
   assert(x >= 0 && y >=0); 
   Macroblock &decompressed_mb = buf_decompressed->get_frame_mb(0, x, y);  
   
      // compute delta and quantize
   predicted_forward(mb, decompressed_mb); 
   if(!printed)
		    print(mb, "compressor first bframe out");

   //write_mb(stream, mb, std::pair(-1, -1));
   mb.vect = std::pair(-1, -1);
   buf_compressed->push_mb(mb);
   predicted_inverse(mb, decompressed_mb);
   buf_decompressed->push_mb(mb);
   if(!printed) {
		    print(mb, "compressor first bframe reconstructed");

		    printed = true;
		   }
}

void flush_buf(OutputBitStream &stream) {
   int mb_written = 0;
   int cap = buf_compressed->mb_in_frame();
	//TODO: remove assert
   assert(buf_compressed->mb_in_frame() == 396);
   //for(auto &mb: buf_compressed->buffer) {
   for(int i: frame_encoding_order){
      // decode all blocks in a frame
      std::cerr << "encoding " << i << "\n";
      for(const Macroblock &mb: buf_compressed->get_frame(i)){
         // push a byte flag on new frames
         if(mb_written == 0) stream.push_byte(1);
         mb_written = (mb_written + 1) % cap;
         write_mb(stream, mb, mb.vect);
         
      	

         
      }
   }
}
int main(int argc, char** argv){

   if (argc < 4){
      terminate:
      std::cout << "Usage: " << argv[0] << " <width> <height> <low/medium/high>" << std::endl;
      exit(1);
   }
   // convert arguments to uints
   u32 width = std::stoi(argv[1]);
   u32 height = std::stoi(argv[2]);
   // add padding
   // https://stackoverflow.com/questions/49903319/simple-way-to-calculate-padding-based-on-modulo-remainder
   width += (-width % 16);
   height += (-height % 16);
   std::string quality{argv[3]};
   if(quality == "low")
      qual = Quality::LOW;
   else if(quality == "medium")
      qual = Quality::MED; 
   else if(quality == "high") 
      qual = Quality::HIGH;
   else goto terminate;
   

   // create instances of reader & output stream
   YUVStreamReader reader {std::cin, width, height};
   OutputBitStream output_stream {std::cout};
   // add the width and height into the stream
   output_stream.push_u32(height);
   output_stream.push_u32(width);
   output_stream.push_byte(qual);
   bool haveIFrame = false;
   // initialize state
   Macroblock mb;
   mb.Y.setZero();
   mb.Cb.setZero();
   mb.Cr.setZero();
   buf_decompressed = FrameBuffer{ width, height };
   buf_compressed   = FrameBuffer{ width, height };

   while (reader.read_next_frame()){
      count++;

      //output_stream.push_byte(1); // TODO: uncomment this when NOT using buffered output
      // push flag to indicate there's a next frame
      YUVFrame420& frame = reader.frame();
      for(int y0 = 0; y0 < height; y0 += 16) {
         for(int x0 = 0; x0 < width; x0 += 16) {
            /* fill blocks */
            for(int y = 0; y < 16; y++) {
               for(int x = 0; x < 16; x++) {
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
            int frame_count = buf_decompressed->frame_idx();
            bool is_p_or_iframe = frame_count % 4 == 0;
            bool is_between_first_and_last_frame = 0 < frame_count && frame_count < 16;
            if(is_p_or_iframe) {
               
               encode_and_write_mb(output_stream, mb, x0, y0);
            } else if(is_between_first_and_last_frame){ 
               encode_and_write_vector_search(output_stream, mb, x0, y0);
            }
            
            
         }
      }
      bool frame_buffer_is_full = buf_decompressed->frame_count() - 1 == 17;
      if(frame_buffer_is_full) {
         
         flush_buf(output_stream);
         buf_compressed->clear();
         buf_decompressed->clear();
         //exit(0);
      }
   }
   //if(buf_compressed->size()) flush_buf(output_stream);
   output_stream.push_byte(0); //Flag to indicate end of data
   output_stream.flush_to_byte();
   return 0;
}
