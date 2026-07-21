#ifndef TYPES_HPP
#define TYPES_HPP
#include <cstdint>
#include <Eigen/Dense>

/* These definitions are more reliable for fixed width types than using "int" and assuming its width */
using u8 = std::uint8_t;
using i8 = std::int8_t;
using i9 = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using Matrix8d = Eigen::Matrix<double, 8, 8>;
using Matrix16d = Eigen::Matrix<double, 16, 16>;
using BlockVect = std::tuple<i16, i16, i8>;
typedef struct {
   Matrix16d Y;
   Matrix8d Cb;
   Matrix8d Cr;
   BlockVect vect;
} Macroblock;
enum FrameType {
   IFRAME,
   PFRAME
};
enum Quality {
   LOW = 0, MED = 1, HIGH = 2, DELTA = 3,
};

#endif
