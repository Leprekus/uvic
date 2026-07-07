#include <iostream>
#include <Eigen/Dense>

// Include your project headers
#include "types.hpp"
#include "dct.hpp"
#include "quantize.hpp"
#include "yuv_pipeline.hpp"

int main() {
   // 1. Define a hardcoded 8x8 pixel block (e.g., a simple gradient)
   Eigen::MatrixXd block(8, 8);
   block << 52, 55, 61, 66, 70, 61, 64, 73,
         63, 59, 55, 90, 109, 85, 69, 72,
         62, 59, 68, 113, 144, 104, 66, 73,
         63, 58, 71, 122, 154, 106, 70, 69,
         67, 61, 68, 104, 126, 88, 68, 70,
         79, 65, 60, 70, 77, 68, 58, 75,
         85, 71, 64, 59, 55, 61, 65, 83,
         87, 79, 69, 68, 65, 76, 78, 94;

   std::cout << "\nOriginal Block:\n" << block << std::endl;

   // 2. Test your DCT implementation
   // (Adjust the function names/types based on your actual dct.hpp)
   Eigen::MatrixXd dct_result = Codec::DCT::forward(block); 
   std::cout << "\nAfter DCT:\n" << dct_result << std::endl;

   // 3. Test your Quantization
   // (Adjust based on your actual quantize.hpp)
   Eigen::MatrixXd quantized = Codec::LumQuant::forward(dct_result);
   std::cout << "\nQuantized Block:\n" << quantized << std::endl;

   // 4. Test Round-trip (Dequantize -> Inverse DCT)
   Eigen::MatrixXd dequantized = Codec::LumQuant::inverse(quantized);
   Eigen::MatrixXd reconstructed = Codec::DCT::inverse(dequantized);
   std::cout << "\nReconstructed Block:\n" << reconstructed << std::endl;

   // 5. Calculate Loss/Error
   Eigen::MatrixXd error = block - reconstructed;
   std::cout << "\nAbsolute Error Matrix:\n" << error.cwiseAbs() << std::endl;


   int count = 0;
   Eigen::MatrixXd M(8,8);
   for(auto frame_view: Codec::YUVPipeline::chunk_frame(16, 16)) {
         for(auto &&[x, y]: frame_view) {
            M(x % 8, y % 8) = x;
            if(count % 8 == 0) std::cout << std::endl;
            std::cout << "(" << x << "," << y << ")";
            count++;
         }
         count = 0;
         std::cout << std::endl;
         for(auto &&[x, y]: frame_view) {
            if(count % 8 == 0) std::cout << std::endl;
            std::cout << "(" << M(x % 8, y % 8) << ")";
            count++;
         }
         count = 0;
         std::cout << std::endl;
        
   }
   return 0;
}
