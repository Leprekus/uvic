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
#include <ranges>
#include <fstream>
#include <array>
#include <string>
#include <cassert>
#include <cstdint>
#include <tuple>

#include <unordered_map>
#include <Eigen/Dense>
#include "utils.hpp"
#include "output_stream.hpp"
#include "yuv_stream.hpp"
#include "input_stream.hpp"
#include "dct.hpp"
#include "quantize.hpp"

/*
 * TODO:
 * - [ ] handle RDO coding enabled
 * - [ ] treat RDO as independent choices
 * - [ ] encode mb
 * */

bool printed = false;
std::optional<FrameBuffer> buf_compressed;
std::optional<FrameBuffer> buf_decompressed;
Quality qual = Quality::MED;
u32 global_width = 0;
u32 global_height = 0;
u32 width = 0;
u32 height = 0;

int fcount = 0;
void write_intra_vector(OutputBitStream &stream, const Macroblock &mb)
{
   auto [x, y, z] = mb.vect;

   // fcount++;
   // if(fcount % buf_compressed->mb_in_frame() == 0) {
   //    std::cerr << " frame " << buf_compressed->frame_count() << " copy " << mb.is_copy << " x " << x << " y " << y << " z " << static_cast<int>(z) << "\n";
   // }
   if (mb.is_copy)
      stream.push_byte(static_cast<u8>(1U));
   else
      stream.push_byte(static_cast<u8>(0U));

   stream.push_byte(static_cast<u8>(x >> 8));
   stream.push_byte(static_cast<u8>(x)); // push high byte

   stream.push_byte(static_cast<u8>(y >> 8));
   stream.push_byte(static_cast<u8>(y));

   stream.push_byte(static_cast<u8>(z));
}
auto write_mb(OutputBitStream &stream, const Macroblock &mb, BlockVect vect)
{

   write_intra_vector(stream, mb);
   if (mb.is_copy)
      return;
   // write the matrix if block is not a copy
   compressed_mb_to_bitstream(mb, stream);
   // std::cerr << "CY\n" <<mb.Y<<"\n";
   // std::cerr << "Cb\n" <<mb.Cb<<"\n";
   // std::cerr << "Cr\n" <<mb.Cr<<"\n";
   // exit(0);
   ////

   // for(auto y = 0; y < 16; y++)
   //    for(auto x = 0; x < 16; x++)
   //       stream.push_byte(static_cast<i8>(mb.Y(x, y)));

   // for(auto y = 0; y < 8; y++)
   //    for(auto x = 0; x < 8; x++)
   //       stream.push_byte(static_cast<i8>(mb.Cb(x, y)));

   // for(auto y = 0; y < 8; y++)
   //    for(auto x = 0; x < 8; x++)
   //       stream.push_byte(static_cast<i8>(mb.Cr(x, y)));
}

Macroblock tmp{
    .Y = Matrix16d::Zero(16, 16),
    .Cb = Matrix8d::Zero(8, 8),
    .Cr = Matrix8d::Zero(8, 8)};
int get_aad(Macroblock &want, Macroblock &have)
{
   return (
       ((want.Y - have.Y).array().abs().sum() +
        (want.Cb - have.Cb).array().abs().sum() +
        (want.Cr - have.Cr).array().abs().sum()) /
       384);
}
BlockVect intra_prediction(Macroblock &mb, auto i, auto x, auto y)
{
   tmp = mb;
   if (x >= 16)
   {
      Macroblock &left = buf_decompressed->get_frame_mb(i, x - 16, y);
      if (get_aad(tmp, left) <= 5)
      {
         predicted_forward(mb, left);
         return BlockVect(-16, 0, -1);
      }
   }
   if (y >= 16)
   {
      Macroblock &above = buf_decompressed->get_frame_mb(i, x, y - 16);
      if (get_aad(tmp, above) <= 5)
      {
         predicted_forward(mb, above);
         return BlockVect(0, -16, -1);
      }
   }
   if (x >= 16 && y >= 16)
   {
      Macroblock &topleft = buf_decompressed->get_frame_mb(i, x - 16, y - 16);
      if (get_aad(tmp, topleft) <= 5)
      {
         predicted_forward(mb, topleft);
         return BlockVect(-16, -16, -1);
      }
   }
   iframe_forward(qual, mb);
   return BlockVect(-1, -1, -1);
}

// sum of absolute difference
auto sad = [](const Macroblock &want, const Macroblock &have)
{
   return (
       (want.Y - have.Y).array().abs().sum() +
       (want.Cb - have.Cb).array().abs().sum() +
       (want.Cr - have.Cr).array().abs().sum());
};
// TODO: cache results
typedef struct Item
{
   int idx, x, y;
   int best_sad;
   bool is_copy = false;
} Item;

int matches = 0;
constexpr int tolerance = 512;
Item inter_block_search(const Macroblock &mb, const int x0, const int y0)
{
   // 1. get the ith block of the previous buffered frames
   int frame_idx = buf_decompressed->frame_count() - 1;                   // exclude currrent frame
   Item it = {.idx = frame_idx, .x = x0, .y = y0, .best_sad = INT32_MAX}; // initialize best item to same block on prev frame
   for (int i = frame_idx; i >= 0; i--)
   {
      const Macroblock &decompressed = buf_decompressed->get_frame_mb(i, x0, y0);
      int curr_sad = sad(mb, decompressed);
      // 2. pick the (frame, block) with the best SAD value
      if (curr_sad < it.best_sad)
      {
         it.best_sad = curr_sad;
         it.idx = i;
      }
   }
   // std::cerr << " sad " << it.best_sad << "\n" << "original " << mb.Y << "\n" << " copy \n" << buf_decompressed->get_frame_mb(it.idx, it.x, it.y).Y; exit(1);
   /*
    * perform a hexagon-search:
    * top - looks two blocks ahead
    * bottom - looks two blocks ahead
    * topL, bottomL, topR, bottomR, looks two blocks ahead over the
    * x-axis and one block ahead over the y-axis in an L shape.
    * This maximizes the distance covered.
    *
    * Every two frames the lookahead distance in each direction is doubled,
    * with the idea that the farther away the reference frame is, the farther
    * away a possible match will be found.
    * */
   constexpr auto cached = [](auto i, auto x, auto y) -> const Macroblock &
   { return buf_decompressed->get_frame_mb(i, x, y); };

   auto &[idx, x, y, best_sad, is_copy] = it;
   is_copy = true;
   if (best_sad <= tolerance)
   {
      matches++;
      return it; // premature exit
   }

   for (int j = idx; j >= 0; j--)
   {

      int lookahead_long = 32;
      int lookahead_short = 16;
      // double the lookahead distance every 2 frames
      // if(!(j & 1)) {
      //   lookahead_long += 32;
      //   lookahead_short += 16;
      //}
      // compare with second Macroblock top
      int curr_sad = sad(mb, cached(j, x, (y - lookahead_long) % global_height));
      if (curr_sad < best_sad)
      {
         y = ((y - lookahead_long) % global_height);
         idx = j;
         best_sad = curr_sad;
         matches++;
         if (curr_sad <= tolerance)
            return it;
      }
      // compare with second Macroblock top-left
      curr_sad = sad(mb, cached(j, (x - lookahead_long) % global_width, (y - lookahead_short) % global_height));
      if (curr_sad < best_sad)
      {
         x = (x - lookahead_long) % global_width;
         y = (y - lookahead_short) % global_height;
         idx = j;
         best_sad = curr_sad;
         matches++;
         if (curr_sad <= tolerance)
            return it;
      }
      // compare with second Macroblock bottom-left
      curr_sad = sad(mb, cached(j, (x - lookahead_long) % global_width, (y + lookahead_short) % global_height));
      if (curr_sad < best_sad)
      {
         x = (x - lookahead_long) % global_width;
         y = (y + lookahead_short) % global_height;
         idx = j;
         best_sad = curr_sad;
         matches++;
         if (curr_sad <= tolerance)
            return it;
      }
      // compare with second Macroblock top-right
      curr_sad = sad(mb, cached(j, (x + lookahead_long) % global_width, (y - lookahead_short) % global_height));
      if (curr_sad < best_sad)
      {
         x = (x + lookahead_long) % global_width;
         y = (y - lookahead_short) % global_height;
         idx = j;
         best_sad = curr_sad;
         matches++;
         if (curr_sad <= tolerance)
            return it;
      }
      // compare with second Macroblock bottom-right
      curr_sad = sad(mb, cached(j, (x + lookahead_long) % global_width, (y + lookahead_short) % global_height));
      if (curr_sad < best_sad)
      {
         x = (x + lookahead_long) % global_width;
         y = (y + lookahead_short) % global_height;
         idx = j;
         best_sad = curr_sad;
         matches++;
         if (curr_sad <= tolerance)
            return it;
      }
      // compare with second Macroblock bottom
      curr_sad = sad(mb, cached(j, x, (y + lookahead_long) % global_height));
      if (curr_sad < best_sad)
      {
         y = (y + lookahead_long) % global_height;
         idx = j;
         best_sad = curr_sad;
         matches++;
         if (curr_sad <= tolerance)
            return it;
      }
   }

   is_copy = false;
   return it;
}

int copies = 0;
int deltas = 0;
void encode_and_buffer_vector_search(OutputBitStream &stream, Macroblock &mb, int x0, int y0)
{

   auto [idx, x, y, best_sad, is_copy] = inter_block_search(mb, x0, y0);
   assert(buf_decompressed->frame_count() >= 1);
   assert(buf_decompressed->frame_count() == buf_compressed->frame_count());
   assert(idx <= buf_decompressed->frame_count() - 1);
   Macroblock &decompressed_mb = buf_decompressed->get_frame_mb(idx, x, y); // get the best match from the inter_block search
   if (is_copy)
   { // if match is good enough reuse the whole block
      copies++;
      // copy the block, update the flag
      Macroblock copy_com = buf_compressed->get_frame_mb(idx, x, y);
      Macroblock copy_dec = buf_decompressed->get_frame_mb(idx, x, y);
      copy_com.is_copy = true;
      // store the deltas from the block that we have
      copy_com.vect = BlockVect(x - x0, y - y0, idx);

      copy_dec.is_copy = true;
      copy_dec.vect = BlockVect(x - x0, y - y0, idx);
      // push blocks into the stream
      buf_compressed->push_mb(copy_com);
      buf_decompressed->push_mb(copy_dec);
   }
   else
   { // otherwise, the delta
      deltas++;
      predicted_forward(mb, decompressed_mb);
      mb.vect = BlockVect(x, y, idx);
      buf_compressed->push_mb(mb);
      predicted_inverse(mb, decompressed_mb);
      buf_decompressed->push_mb(mb);
   }
}

int count = 0;
void encode_iframe(Macroblock &mb, auto x, auto y)
{
   count++;
   // Create a vector for intra-prediction if applicable
   int frame_idx = buf_compressed->frame_count();
   mb.vect = intra_prediction(mb, frame_idx, x, y);
   buf_compressed->push_mb(mb);
   intra_reconstruct(buf_decompressed, qual, mb, frame_idx, x, y, mb.vect);
   buf_decompressed->push_mb(mb);
   // Create a fingerprint for the Macroblock using the compressed data
}
auto encode_and_buffer_mb(OutputBitStream &stream, Macroblock &mb, auto x, auto y)
{

   bool is_first_frame = buf_compressed->frame_count() < 1;
   if (is_first_frame)
   { // encode I-frame
      encode_iframe(mb, x, y);
      assert(!mb.is_copy);
   }
   else
   { // encode P-frame
      encode_and_buffer_vector_search(stream, mb, x, y);
   }
}

int written = 0;
/*
 * the functions push bits with the following meaning:
 * 0 - no delta
 * 0 - no repetitions
 *
 * */

void write_frames_to_stream(OutputBitStream &stream)
{
   // TODO: remove assert
   // assert(buf_compressed->mb_in_frame() == 396);
   assert(buf_compressed->frame_count() <= 4);
   int mb_written = 0;
   int cap = buf_decompressed->mb_in_frame();
   for (int i = 0; i < buf_compressed->frame_count(); i++)
   {
      int x0 = 0;
      int y0 = 0;
      stream.push_byte(1);
      for (const Macroblock &mb : buf_compressed->get_frame(i))
      {
         written++;
         // if(written == 396 || written == 3960)  {
         //    std::cerr << "Y\n"<<mb.Y << "\n"<<"Cb"<<mb.Cb<<"\n"<<"Cr"<<mb.Cr<<"\n";

         //}
         // push a byte flag on new frames
         // compress_mb(mb, stream);
         write_mb(stream, mb, mb.vect);
      }
      stream.flush_to_byte();
   }
}
void parse_args(int argc, char* argv[]) {
   if (argc < 4)
   {
terminate:
      std::cout << "Usage: " << argv[0] << " <width> <height> <low/medium/high>" << std::endl;
      exit(1);
   }
   // convert arguments to uints
   width = std::stoi(argv[1]);
   height = std::stoi(argv[2]);
   // add padding
   // https://stackoverflow.com/questions/49903319/simple-way-to-calculate-padding-based-on-modulo-remainder
   width += (-width % 16);
   height += (-height % 16);
   global_width = width;
   global_height = height;
   std::string quality{argv[3]};
   if (quality == "low")
      qual = Quality::LOW;
   else if (quality == "medium")
      qual = Quality::MED;
   else if (quality == "high")
      qual = Quality::HIGH;
   else
      goto terminate;
}

int main(int argc, char **argv)
{

   parse_args(argc, argv); 
   // create instances of reader & output stream
   YUVStreamReader reader{std::cin, width, height};
   OutputBitStream output_stream{std::cout};
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
   buf_decompressed = FrameBuffer{width, height};
   buf_compressed = FrameBuffer{width, height};
   while (reader.read_next_frame())
   {

      // output_stream.push_byte(1); // TODO: uncomment this when NOT using buffered output
      //  push flag to indicate there's a next frame
      YUVFrame420 &frame = reader.frame();
      for (int y0 = 0; y0 < height; y0 += 16)
      {
         for (int x0 = 0; x0 < width; x0 += 16)
         {
            /* fill blocks */
            for (int y = 0; y < 16; y++)
            {
               for (int x = 0; x < 16; x++)
               {
                  /* fill Y block */
                  mb.Y(x, y) = frame.Y(x0 + x, y0 + y);
                  /* fill Cb, Cr blocks */
                  if (x < 8 && y < 8)
                  {
                     mb.Cb(x, y) = frame.Cb(x0 / 2 + x, y0 / 2 + y);
                     mb.Cr(x, y) = frame.Cr(x0 / 2 + x, y0 / 2 + y);
                  }
               }
            }
            /* create an I-Frame every 64 frames */
            bool buffer_is_full = buf_compressed->frame_count() >= 4;
            if (buffer_is_full)
               throw std::runtime_error("buf_compressed overflow");

            encode_and_buffer_mb(output_stream, mb, x0, y0);
         }
      }
      bool buffer_is_full = buf_compressed->frame_count() >= 4;
      if (buffer_is_full)
      {
         write_frames_to_stream(output_stream);
         buf_compressed->clear();
         buf_decompressed->clear();
         // exit(0);
      }
   }
   if (buf_compressed->size())
      write_frames_to_stream(output_stream);
   std::cerr << "written " << written << "\n";
   output_stream.push_byte(0); // Flag to indicate end of data
   output_stream.flush_to_byte();
   std::cerr << "matches " << matches << "\n";
   std::cerr << "copies " << copies << "\n";
   std::cerr << "deltas " << deltas << "\n";
   return 0;
}
