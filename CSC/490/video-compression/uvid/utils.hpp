#include "types.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <algorithm>
#include <span>

static const int frame_traversal_order[17] = {
   0, 4, 1, 2, 3,
   8, 5, 6, 7,
   12, 9, 10, 11,
   16, 13, 14, 15
};

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

const double low = 1.618;
const double high = 0.618;
static const Matrix8d QYMed {
      {16, 11, 10, 16, 24, 40, 51, 61},
      {12, 12, 14, 19, 26, 58, 60, 55,},
      {14, 13, 16, 24, 40, 57, 69, 56},
      {14, 17, 22, 29, 51, 87, 80, 62},
      {18, 22, 37, 56, 68, 109, 103, 77},
      {25, 35, 55, 64, 81, 104, 113, 92},
      {49, 64, 78, 87, 103, 121, 120, 101},
      {72, 92, 95, 98, 112, 100, 103, 99},
   };
static const Matrix8d QYLow = (QYMed * 1.618).array().round().matrix(); 
static const Matrix8d QYHigh = (QYMed * 0.618).array().round().matrix(); 

static const Matrix8d QCMed {
      {17, 18, 24, 47, 99, 99, 99, 99},
      {18, 21, 26, 66, 99, 99, 99, 99},
      {24, 26, 56, 99, 99, 99, 99, 99},
      {47, 66, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
      {99, 99, 99, 99, 99, 99, 99, 99},
   };
static const Matrix8d QCLow = (QCMed * 2).array().round().matrix();
static const Matrix8d QCHigh = (QCMed * 0.90).array().round().matrix();

static const Matrix8d QYDeltas = (QYMed * 0.80).array().round().matrix();
static const Matrix8d QCDeltas = (QCMed * 1).array().round().matrix();
static const Matrix8d QuantY[4] = {
   QYLow, QYMed, QYHigh, QYDeltas
};
static const Matrix8d QuantC[4] = {
   QCLow, QCMed, QCHigh, QCDeltas
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

static void quantize_forward(const Matrix8d &Q, Matrix8dRef D) {
   D = D.cwiseQuotient(Q);
}

static void quantize_inverse(const Matrix8d &Q, Matrix8dRef T) {
   T = T.cwiseProduct(Q);
}
static void transform_and_quantize(const Matrix8d &Q, Matrix8dRef block) {
   //block.array() -= 128;
   dct_forward(block);
   // choose the correct quantization matrix for
   // Y / (Cb | Cr) blocks
   quantize_forward(Q, block);
   //block  = block.array().round().cwiseMax(-127).cwiseMin(128);
}

static void reconstruct_block(const Matrix8d &Q, Matrix8dRef block) {
   // choose the correct quantization matrix for
   // Y / (Cb | Cr) blocks
   quantize_inverse(Q, block);
   dct_inverse(block);
   block = block.array().round();
   // restore the offset peformed in the forward() operation
   
}


class FrameBuffer {
   private:
      u32 width, height, capacity;
   public:
      std::vector<Macroblock> buffer;
      FrameBuffer(u32 w, u32 h) {
         width = w / 16;
         height = h / 16;
         const int macroblocks_per_frame = ceil(((double)width * height)/384); 
         capacity = macroblocks_per_frame * 17;
         buffer.reserve(capacity);
      }
      Macroblock &get_mb(int x, int y) {
         /*
          * calculate index as:
          * 1. round to neareast block (multiple of 16): x = x + (-x mod 16)
          * 2. divide over 16 (>>4) to map a pixel to a block index
          * 3. multiply width * y + x to get the idx for the ith macroblock
          * */
         x = (x + (-x & 15)) >> 4;
         y = (y + (-y & 15)) >> 4;

         int frame_offset = std::max((int)frame_count() * (int)width * (int)height - 1, 0);
         return buffer.at(frame_offset + width * y + x);
      }
      void push_mb(const Macroblock &mb) {
         buffer.push_back(mb);
      }
      std::span<const Macroblock> get_frame(int i) {
         int start = width * height * i;
         int step = (width * height);
         assert(start + step <= size());
         return std::span<const Macroblock>(&buffer[start], step);
      }
      int mb_in_frame() {
         return width * height;
      }
      void clear() {
         buffer.clear();
      }
      size_t size() {
         return buffer.size();
      }
      u32 frame_count() { 
         u32 count = buffer.size() / (width * height);
         return count;
      }
};

static void iframe_forward(Quality qual, Macroblock &mb) {
   // center data around -127 and 128
   mb.Y.array() -= 128;
   mb.Cb.array() -= 128;
   mb.Cr.array() -= 128;
   transform_and_quantize(QuantY[qual], mb.Y.block<8, 8>(0, 0)); // Top-Left
   transform_and_quantize(QuantY[qual], mb.Y.block<8, 8>(0, 8)); // Top-Right
   transform_and_quantize(QuantY[qual], mb.Y.block<8, 8>(8, 0)); // Bottom-Left
   transform_and_quantize(QuantY[qual], mb.Y.block<8, 8>(8, 8)); // Bottom-Right
                                                      
   transform_and_quantize(QuantC[qual], mb.Cb);
   transform_and_quantize(QuantC[qual], mb.Cr);
   mb.Y  = mb.Y.array().round().cwiseMax(-127).cwiseMin(128);
   mb.Cr = mb.Cr.array().round().cwiseMax(-127).cwiseMin(128);
   mb.Cb = mb.Cb.array().round().cwiseMax(-127).cwiseMin(128);

}
static void iframe_inverse(Quality qual, Macroblock &mb) {
   reconstruct_block(QuantY[qual], mb.Y.block<8, 8>(0, 0)); // Top-Left
   reconstruct_block(QuantY[qual], mb.Y.block<8, 8>(0, 8)); // Top-Right
   reconstruct_block(QuantY[qual], mb.Y.block<8, 8>(8, 0)); // Bottom-Left
   reconstruct_block(QuantY[qual], mb.Y.block<8, 8>(8, 8)); // Bottom-Right
                                                              
   reconstruct_block(QuantC[qual], mb.Cb);
   reconstruct_block(QuantC[qual], mb.Cr);

   mb.Y.array()  += 128;
   mb.Cb.array() += 128;
   mb.Cr.array() += 128;
   // clamp values between 0 and 255 
   mb.Y =  mb.Y.cwiseMax(0).cwiseMin(255);
   mb.Cb = mb.Cb.cwiseMax(0).cwiseMin(255);
   mb.Cr = mb.Cr.cwiseMax(0).cwiseMin(255);

}

static void predicted_forward(Macroblock &mb, Macroblock &decompressed_mb) {
   mb.Y  -=  decompressed_mb.Y;
   mb.Cb -= decompressed_mb.Cb;
   mb.Cr -= decompressed_mb.Cr;
   transform_and_quantize(QuantY[Quality::DELTA], mb.Y.block<8, 8>(0, 0)); // Top-Left
   transform_and_quantize(QuantY[Quality::DELTA], mb.Y.block<8, 8>(0, 8)); // Top-Right
   transform_and_quantize(QuantY[Quality::DELTA], mb.Y.block<8, 8>(8, 0)); // Bottom-Left
   transform_and_quantize(QuantY[Quality::DELTA], mb.Y.block<8, 8>(8, 8)); // Bottom-Right
                                                      
   transform_and_quantize(QuantC[Quality::DELTA], mb.Cb);
   transform_and_quantize(QuantC[Quality::DELTA], mb.Cr);
   mb.Y  = mb.Y.array().round().cwiseMax(-128).cwiseMin(127);
   mb.Cr = mb.Cr.array().round().cwiseMax(-128).cwiseMin(127);
   mb.Cb = mb.Cb.array().round().cwiseMax(-128).cwiseMin(127);
}
static void predicted_inverse(Macroblock &mb, Macroblock &decompressed_mb) {
   reconstruct_block(QuantY[Quality::DELTA], mb.Y.block<8, 8>(0, 0)); // Top-Left
   reconstruct_block(QuantY[Quality::DELTA], mb.Y.block<8, 8>(0, 8)); // Top-Right
   reconstruct_block(QuantY[Quality::DELTA], mb.Y.block<8, 8>(8, 0)); // Bottom-Left
   reconstruct_block(QuantY[Quality::DELTA], mb.Y.block<8, 8>(8, 8)); // Bottom-Right
                                                              
   reconstruct_block(QuantC[Quality::DELTA], mb.Cb);
   reconstruct_block(QuantC[Quality::DELTA], mb.Cr);
   mb.Y  +=  decompressed_mb.Y;
   mb.Cb += decompressed_mb.Cb;
   mb.Cr += decompressed_mb.Cr;
   mb.Y =  mb.Y.cwiseMax(0).cwiseMin(255);
   mb.Cb = mb.Cb.cwiseMax(0).cwiseMin(255);
   mb.Cr = mb.Cr.cwiseMax(0).cwiseMin(255);
}

void intra_reconstruct(
      std::optional<FrameBuffer> &buffer, Quality qual,
      Macroblock &mb, 
      int x0, int y0,
      std::pair<char, char> offset);
void print(Macroblock &mb, std::string tag);
