#ifndef TYPES_HPP
#define TYPES_HPP
#include <cstdint>
#include <Eigen/Dense>

/* These definitions are more reliable for fixed width types than using "int" and assuming its width */
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using Matrix8d = Eigen::Matrix<double, 8, 8>;
using Matrix16d = Eigen::Matrix<double, 16, 16>;
typedef struct {
   Matrix16d Y;
   Matrix8d Cb;
   Matrix8d Cr;
   std::pair<char, char> vect;
} Macroblock;
enum FrameType {
   IFRAME,
   PFRAME
};
enum Quality {
   LOW = 0, MED = 1, HIGH = 2, DELTA = 3,
};

#endif
