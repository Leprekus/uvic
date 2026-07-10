#ifndef YUV_PIPELINE_HPP
#define YUV_PIPELINE_HPP
#include <ranges>
#include <Eigen/Dense>
#include "types.hpp"
namespace Codec {
   class YUVPipeline {
      public:
         

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
}
 
#endif
