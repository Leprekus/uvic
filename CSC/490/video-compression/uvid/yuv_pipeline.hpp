#ifndef YUV_PIPELINE_HPP
#define YUV_PIPELINE_HPP
#include <ranges>
#include <Eigen/Dense>
#include "types.hpp"
#include "dct.hpp"
#include "quantize.hpp"
#include "yuv_stream.hpp"

namespace Codec {
   enum BlockType { YBlock, CBlock };
   template <const BlockType type>

   class YUVPipeline {
       
      public:
          Matrix8d &DCTForward(Matrix8d &block) {
            // center data around -127 and 128
            block = block.array() - 128;
            block = Codec::DCT::forward(block);
            // choose the correct quantization matrix for
            // Y / (Cb | Cr) blocks
            if(type == BlockType::YBlock)
               block = Codec::QuantizeYb::forward(block);
            else if(type == BlockType::CBlock)
               block = Codec::QuantizeCb::forward(block);
            block = block.array().round();
            return block;
         } 
         Matrix8d &DCTInverse(Matrix8d &block) {
            // choose the correct quantization matrix for
            // Y / (Cb | Cr) blocks
            if(type == BlockType::YBlock)
               block = Codec::QuantizeYb::inverse(block);
            else if(type == BlockType::CBlock)
               block = Codec::QuantizeCb::inverse(block);
            block = Codec::DCT::inverse(block);
            block = block.array().round();
            // restore the offset peformed in the forward() operation
            block = block.array() + 128;
            // clamp values between 0 and 255 
            block = block.cwiseMax(0).cwiseMin(255);
            return block;
         }
         template <typename ChunkIterable, typename BlockRead, typename BlockProcess, typename BlockWrite>
         auto process_block(
               ChunkIterable chunks,
               BlockRead block_read,
               BlockProcess block_process,
               BlockWrite block_write
               ) {
            for(auto &&frame: chunks){
               for(auto &&[x, y]: frame)
                  block_read(x, y);
               block_process();
               for(auto &&[x, y]: frame)
                  block_write(x, y);
            }
         }

         /*
          * returns an array of views,
          * each view contains all the coordinates
          * for a distinct 8x8 block in the width * height grid
          * */
         auto chunk_frame(u32 width, u32 height, u32 chunk_size){
            auto blocks_y = std::views::iota(0U, height) | std::views::chunk(chunk_size); 
            auto blocks_x = std::views::iota(0U, width)  | std::views::chunk(chunk_size); 
            auto blocks = std::views::cartesian_product(blocks_x, blocks_y);
            return blocks | std::views::transform([](auto &&blocks){
               auto [grid_x, grid_y] = blocks;
               return std::views::cartesian_product(grid_x, grid_y);
            });
         }

         

         
         template<
            typename YRead, typename CRead, 
            typename MacroProcess, typename MacroWrite
               >
         auto process_macroblock(
               u32 width, u32 height,
               YRead y_read,
               CRead c_read,
               MacroProcess macro_process,
               MacroWrite macro_write
               ) {
            // TODO: add max if this fails
            for(auto i = 0; i < height; i += 16) {
               for(auto j = 0; j < width; j += 16) {
                  // Read into Y 16x16 block
                  for(auto y = i; y < i + 16; y++) 
                     for(auto x = j; x < j + 16; x++)
                      y_read(x, y);
                  // C 8x8 block
                  for(auto y = i/2; y < i/2 + 8; y++) 
                     for(auto x = j/2; x < j/2 + 8; x++)
                        c_read(x, y);
                  
                  macro_process(j, i);
                  macro_write(j, i);
               }
            }  

         }
         auto vector_search(
               auto x0, auto y0,
               YUVFrame420 &compressed_frame, 
               Matrix16d &YM, Matrix8d &Cb, Matrix8d &Cr) {
            // Y processing
            for(auto y = y0; y < y0 + 16; y++)
               for(auto x = x0; x < x0 + 16; x++)
                  // store delta
                  YM(x % 16, y % 16) = YM(x % 16, y % 16) - compressed_frame.Y(x, y);
            for(auto y = y0; y < y0 + 16; y++)
               for(auto x = x0; x < x0 + 16; x++)
                  // quantize and round
                  YM = (YM / 2).array().round();

            // Cb, Cr processing
            for(auto y = y0/2; y < y0/2 + 16; y++)
               for(auto x = x0/2; x < x0/2 + 16; x++) {
                  // store delta
                  Cb(x % 8, y % 8) = Cb(x % 8, y % 8) - compressed_frame.Cb(x, y);
                  Cr(x % 8, y % 8) = Cr(x % 8, y % 8) - compressed_frame.Cr(x, y);
               }
            for(auto y = y0/2; y < y0/2 + 16; y++)
               for(auto x = x0/2; x < x0/2 + 16; x++) {
                  // quantize and round
                  Cb = (Cb / 2).array().round();
                  Cr = (Cr / 2).array().round();
               }
         }
   auto restore_delta(
               auto x0, auto y0,
               YUVFrame420 &compressed_frame, 
               Matrix16d &YM, Matrix8d &Cb, Matrix8d &Cr) {
            // Y processing
            for(auto y = y0; y < y0 + 16; y++)
               for(auto x = x0; x < x0 + 16; x++)
                  // store delta
                  YM(x % 16, y % 16) = YM(x % 16, y % 16) + compressed_frame.Y(x, y);
            for(auto y = y0; y < y0 + 16; y++)
               for(auto x = x0; x < x0 + 16; x++)
                  // dequantize, round, and clamp
                  YM = (YM * 2).array().round().cwiseMax(0).cwiseMin(255);

            // Cb, Cr processing
            for(auto y = y0/2; y < y0/2 + 16; y++)
               for(auto x = x0/2; x < x0/2 + 16; x++) {
                  // store delta
                  Cb(x % 8, y % 8) = Cb(x % 8, y % 8) + compressed_frame.Cb(x, y);
                  Cr(x % 8, y % 8) = Cr(x % 8, y % 8) + compressed_frame.Cr(x, y);
               }
            for(auto y = y0/2; y < y0/2 + 16; y++)
               for(auto x = x0/2; x < x0/2 + 16; x++) {
                  // quantize and round
                  Cb = (Cb * 2).array().round().cwiseMax(0).cwiseMin(255);
                  Cr = (Cr * 2).array().round().cwiseMax(0).cwiseMin(255);
               }
         }

   };
   using DCTTransformYBlock = YUVPipeline<BlockType::YBlock>;
   using DCTTransformCBlock = YUVPipeline<BlockType::CBlock>;

}
 
#endif
