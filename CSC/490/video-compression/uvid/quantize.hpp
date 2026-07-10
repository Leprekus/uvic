#ifndef QUANTIZE_HPP 
#define QUANTIZE_HPP
#include "types.hpp"
#include <cassert>
#include <iostream>


#include <Eigen/Dense>
      
namespace Codec {
   static const Matrix8d QYb {
      {16, 11, 10, 16, 24, 40, 51, 61},
      {12, 12, 14, 19, 26, 58, 60, 55,},
      {14, 13, 16, 24, 40, 57, 69, 56},
      {14, 17, 22, 29, 51, 87, 80, 62},
      {18, 22, 37, 56, 68, 109, 103, 77},
      {25, 35, 55, 64, 81, 104, 113, 92},
      {49, 64, 78, 87, 103, 121, 120, 101},
      {72, 92, 95, 98, 112, 100, 103, 99},
   };
   static const Matrix8d QCb {
      {17, 18, 24, 47, 99, 99, 99, 99},
      {18, 21, 26, 66, 99, 99, 99, 99},
      {24, 26, 56, 99, 99, 99, 99, 99},
      {47, 66, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
   };
   template <const Matrix8d &QMatrix>
   
   class Quantize {
      public:
         static Matrix8d forward(Matrix8d D) {
            D = D.cwiseQuotient(QMatrix);
            D = D.array().round(); // round before writing to bitstream
            if(!((-128 <= D.array()).all() && (D.array() <= 127).all())){
            std::cerr << "offender" << std::endl;
            std::cerr << D;
            assert((-128 <= D.array()).all() && (D.array() <= 127).all());
            }
            assert((-128 <= D.array()).all() && (D.array() <= 127).all());
            return D;
         }
         static Matrix8d inverse(Matrix8d T) {
            assert((-128 <= T.array()).all() && (T.array() <= 127).all());
            T = T.cwiseProduct(QMatrix);
            return T;
         }
   };
   using QuantizeYb = Quantize<QYb>;
   using QuantizeCb = Quantize<QCb>;

}

#endif
