#include "types.hpp"
class Macroblock {
   private:
      Matrix16d Y;
      Matrix8d Cb;
      Matrix8d Cr;
   public:
      void set_y(Matrix16d &newY) {
         Y = newY;
      }
      void set_cb(Matrix8d &newCb) {
         Cb = newCb;
      }
      void set_cr(Matrix8d &newCr) {
         Cr = newCr;
      }
};
