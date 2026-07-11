#include "types.hpp"
#include <Eigen/Dense>

static const Matrix8d C {
   {0.3535533905932738,0.3535533905932738,0.3535533905932738,0.3535533905932738,0.3535533905932738,0.3535533905932738,0.3535533905932738,0.3535533905932738,},
      {0.4903926402016152,0.4157348061512726,0.27778511650980114,0.09754516100806417,-0.0975451610080641,-0.277785116509801,-0.4157348061512727,-0.4903926402016152,},
      {0.46193976625564337,0.19134171618254492,-0.19134171618254486,-0.46193976625564337,-0.4619397662556434,-0.19134171618254517,0.191341716182545,0.46193976625564326,},
      {0.4157348061512726,-0.0975451610080641,-0.4903926402016152,-0.2777851165098011,0.2777851165098009,0.4903926402016152,0.09754516100806439,-0.41573480615127256,},
      {0.3535533905932738,-0.35355339059327373,-0.35355339059327384,0.3535533905932737,0.35355339059327384,-0.35355339059327334,-0.35355339059327356,0.3535533905932733,},
      {0.27778511650980114,-0.4903926402016152,0.09754516100806415,0.41573480615127273,-0.41573480615127256,-0.09754516100806401,0.4903926402016153,-0.27778511650980076,},
      {0.19134171618254492,-0.4619397662556434,0.46193976625564326,-0.19134171618254495,-0.19134171618254528,0.46193976625564337,-0.4619397662556432,0.19134171618254478,},
      {0.09754516100806417,-0.2777851165098011,0.41573480615127273,-0.4903926402016153,0.4903926402016152,-0.4157348061512725,0.27778511650980076,-0.09754516100806429,},
};
static const Matrix8d QYBlock {
      {16, 11, 10, 16, 24, 40, 51, 61},
      {12, 12, 14, 19, 26, 58, 60, 55,},
      {14, 13, 16, 24, 40, 57, 69, 56},
      {14, 17, 22, 29, 51, 87, 80, 62},
      {18, 22, 37, 56, 68, 109, 103, 77},
      {25, 35, 55, 64, 81, 104, 113, 92},
      {49, 64, 78, 87, 103, 121, 120, 101},
      {72, 92, 95, 98, 112, 100, 103, 99},
   };
   static const Matrix8d QCbBlock {
      {17, 18, 24, 47, 99, 99, 99, 99},
      {18, 21, 26, 66, 99, 99, 99, 99},
      {24, 26, 56, 99, 99, 99, 99, 99},
      {47, 66, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
   };
   

enum BlockType {
   YBLOCK, CBLOCK
};
using Matrix8dRef = Eigen::Ref<Matrix8d, 0, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>>;
static void dct_forward(Matrix8dRef A) {
   A =  C * A * C.transpose();
}
static void dct_inverse(Matrix8dRef D) {
   D = C.transpose() * D * C;
}

template <const Matrix8d &QMatrix>
static void quantize_forward(Matrix8dRef D) {
   D = D.cwiseQuotient(QMatrix);
}

template <const Matrix8d &QMatrix>
static void quantize_inverse(Matrix8dRef T) {
   T = T.cwiseProduct(QMatrix);
}
template <const Matrix8d &QMatrix>
static void transform_and_quantize(Matrix8dRef block) {
   // center data around -127 and 128
   block = block.array() - 128;
   dct_forward(block);
   // choose the correct quantization matrix for
   // Y / (Cb | Cr) blocks
   quantize_forward<QMatrix>(block);
   block = block.array().round();
}

template <const Matrix8d &QMatrix>
static void reconstruct_block(Matrix8dRef block) {
   // choose the correct quantization matrix for
   // Y / (Cb | Cr) blocks
   quantize_inverse<QMatrix>(block);
   dct_inverse(block);
   block = block.array().round();
   // restore the offset peformed in the forward() operation
   block = block.array() + 128;
   // clamp values between 0 and 255 
   block = block.cwiseMax(0).cwiseMin(255);
}
