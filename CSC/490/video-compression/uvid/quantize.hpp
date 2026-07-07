#ifndef QUANTIZE_HPP 
#define QUANTIZE_HPP
#include "types.hpp"
#include <cassert>
#include <iostream>


#include <Eigen/Dense>
      
namespace Codec {
   static const Eigen::MatrixXd QLum {
      {16, 11, 10, 16, 24, 40, 51, 61},
      {12, 12, 14, 19, 26, 58, 60, 55,},
      {14, 13, 16, 24, 40, 57, 69, 56},
      {14, 17, 22, 29, 51, 87, 80, 62},
      {18, 22, 37, 56, 68, 109, 103, 77},
      {25, 35, 55, 64, 81, 104, 113, 92},
      {49, 64, 78, 87, 103, 121, 120, 101},
      {72, 92, 95, 98, 112, 100, 103, 99},
   };
   static const Eigen::MatrixXd QChrom {
      {17, 18, 24, 47, 99, 99, 99, 99},
      {18, 21, 26, 66, 99, 99, 99, 99},
      {24, 26, 56, 99, 99, 99, 99, 99},
      {47, 66, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
   };
   template <const Eigen::MatrixXd &QMatrix>
   
   class Quantize {
      private:
         inline static Eigen::MatrixXd tmp{8, 8};
         inline static bool printed = false;

      public:
         static Eigen::MatrixXd forward(Eigen::MatrixXd D) {
            
            D = D.cwiseQuotient(QMatrix);
            D = D.array().round();
            if(!printed) {
               printed = true;
               std::cerr << "Quantized Compressor DCT" << std::endl;
               std::cerr << D << std::endl;
            }
            assert((-128 <= D.array()).all() && (D.array() <= 128).all());
            return D;
         }
         static Eigen::MatrixXd inverse(Eigen::MatrixXd T) {
            if(!printed) {
               printed = true;
               std::cerr << "Quantized Decompressor DCT" << std::endl;
               std::cerr << T << std::endl;
            }
            assert((-128 <= T.array()).all() && (T.array() <= 128).all());
            T = T.cwiseProduct(QMatrix);
            
            return T;
         }
   };
   using LumQuant = Quantize<QLum>;
   using ChromQuant = Quantize<QChrom>;

}

#endif
