#ifndef QUANTIZE_HPP 
#define QUANTIZE_HPP


#include <Eigen/Dense>
      
namespace Codec {
   static const Eigen::MatrixXd Q {
      {16, 11, 10, 16, 24, 40, 51, 61},
      {12, 12, 14, 19, 26, 58, 60, 55,},
      {14, 13, 16, 24, 40, 57, 69, 56},
      {14, 17, 22, 29, 51, 87, 80, 62},
      {18, 22, 37, 56, 68, 109, 103, 77},
      {25, 35, 55, 64, 81, 104, 113, 92},
      {49, 64, 78, 87, 103, 121, 120, 101},
      {72, 92, 95, 98, 112, 100, 103, 99},
   };
   class Quantize {
      private:
         inline static Eigen::MatrixXd tmp{8, 8};

      public:
         static Eigen::MatrixXd &forward(Eigen::MatrixXd &D) {
            return D;
            tmp = D.cwiseQuotient(Q);
            D = tmp.array().round();
            assert((D.array() <= 255).all());
            return D;
         }
         static Eigen::MatrixXd &inverse(Eigen::MatrixXd &T) {
            return T;
            tmp = T.cwiseProduct(Q);
            T = tmp;
            return T;
         }
   };
   
}

#endif
