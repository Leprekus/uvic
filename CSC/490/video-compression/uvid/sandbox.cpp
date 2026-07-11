#include <iostream>
#include <Eigen/Dense>

// Include your project headers
#include "types.hpp"
#include "dct.hpp"
#include "quantize.hpp"
#include "yuv_pipeline.hpp"

auto Ypipeline = Codec::YUVPipeline<Codec::BlockType::YBlock>();
auto test_pipeline(Matrix8d &block) {
   std::cout << "original" << std::endl;
   std::cout << block << std::endl;

   std::cout << "forward" << std::endl;
   block = Ypipeline.DCTForward(block);
   std::cout << block << std::endl;

   std::vector<u8>stream;
   for (auto i = 0 ; i < block.rows(); i++) {
      for(auto j = 0; j < block.cols(); j++) {
         stream.push_back(block(i, j));
      }
   }
   Matrix8d D;
   for (auto i = 0 ; i < block.rows(); i++) {
      for(auto j = 0; j < block.cols(); j++) {
         D(i, j) = static_cast<char>(stream.at(i * 8 + j));
      }
   }

   D = Ypipeline.DCTInverse(D);
   std::cout << "reconstructed" << std::endl;
   std::cout << D << std::endl;
}
int main() {
   // 1. Define a hardcoded 8x8 pixel block (e.g., a simple gradient)
   Matrix8d block(8, 8);
   block << 
         255, 255,255,255,255,255,255,255,
         255, 255,255,255,255,255,255,255,
         255, 255,255,255,255,255,255,255,
         255, 255,255,255,255,255,255,255,
         255, 255,255,255,255,255,255,255,
         255, 255,255,255,255,255,255,255,
         255, 255,255,255,255,255,255,255,
         255, 255,255,255,255,255,255,255,
   //test_pipeline(block); 
   block << 
         0, 0,0,0,0,0,0,0,
         0, 0,0,0,0,0,0,0,
         0, 0,0,0,0,0,0,0,
         0, 0,0,0,0,0,0,0,
         0, 0,0,0,0,0,0,0,
         0, 0,0,0,0,0,0,0,
         0, 0,0,0,0,0,0,0,
         0, 0,0,0,0,0,0,0,
   //test_pipeline(block); 
 block << 
         52, 55,61,66,70, 61,64,73,
         63, 59,55,90,109,85,69,72,
         62, 59,68,113,144,104,66,73,
         63, 58,71,122,154,106,70,69,
         67, 61,68,104,126,88,68,70,
         79, 65,60, 70,77,68,58,75,
         85, 71,64,59,55,61,65,83,
         87, 79,69,68,65,76,78,94,
   test_pipeline(block); 
   
      return 0;
   }
