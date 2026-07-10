#ifndef YUV_PIPELINE_HPP
#define YUV_PIPELINE_HPP
#include <ranges>
#include <Eigen/Dense>
#include "types.hpp"
#include "dct.hpp"
#include "quantize.hpp"

namespace Codec {
   enum BlockType { YBlock, CBlock };
   template <const BlockType type>

   class YUVPipeline {
       
      public:
         static Matrix8d &DCTForward(Matrix8d &block) {
            block = block.array() - 128;
            block = Codec::DCT::forward(block);
            if(type == BlockType::YBlock)
               block = Codec::QuantizeYb::forward(block);
            else if(type == BlockType::CBlock)
               block = Codec::QuantizeCb::forward(block);
            block = block.array().round();
            return block;
         } 
         static Matrix8d &DCTInverse(Matrix8d &block) {
            if(type == BlockType::YBlock)
               block = Codec::QuantizeYb::inverse(block);
            else if(type == BlockType::CBlock)
               block = Codec::QuantizeCb::inverse(block);
            block = Codec::DCT::inverse(block);
            block = block.array().round();
            block = block.array() + 128;
            block = block.cwiseMax(0).cwiseMin(255);
            //block = block.cwiseMin(0).cwiseMax(255);
            return block;
         }

         /*
          * returns an array of views,
          * each view contains all the coordinates
          * for a distinct 8x8 block in the width * height grid
          * */
         static auto chunk_frame(u32 width, u32 height){
            auto blocks_y = std::views::iota(0U, height) | std::views::chunk(8); 
            auto blocks_x = std::views::iota(0U, width)  | std::views::chunk(8); 
            auto blocks = std::views::cartesian_product(blocks_x, blocks_y);
            return blocks | std::views::transform([](auto &&blocks){
               auto [grid_x, grid_y] = blocks;
               return std::views::cartesian_product(grid_x, grid_y);
            });
         }

         
         static auto block_transformation(
               u32 width, u32 height,
               std::function<void(u32 &x, u32 &y)> block_read,
               std::function<void()> block_process,
               std::function<void(u32 &x, u32 &y)> block_write

               ) {
            for (u32 y0 = 0; y0 < height; y0 += 8) {
               for(u32 x0 = 0; x0 < width; x0 += 8) {
                  // fill up 8x8 block
                  for(u32 y = y0; y < y0 + 8; y++)
                     for(u32 x = x0; x < x0 + 8; x++)
                        // bitsream  
                        block_read(x, y);
                  // write 8x8 into the bitstream
                  block_process();
                  for(u32 y = y0; y < y0 + 8; y++)
                     for(u32 x = x0; x < x0 + 8; x++)
                        block_write(x, y);
               }
            }

         }

   };
   using DCTTransformYBlock = YUVPipeline<BlockType::YBlock>;
   using DCTTransformCBlock = YUVPipeline<BlockType::CBlock>;

}
 
#endif
