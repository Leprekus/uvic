#include "utils.hpp"
void intra_reconstruct(
      std::optional<FrameBuffer> &buffer, Quality qual,
      Macroblock &mb, 
      int x0, int y0,
      std::pair<i8, i8> offset) {
   auto [x, y] = offset;
   if(x == y && y == -1) {
      iframe_inverse(qual, mb);
      return;
   };
   Macroblock &decompressed = buffer->get_mb(x0 + x, y0 + y);
   predicted_inverse(mb, decompressed);

}

extern bool printed;
void print(Macroblock &mb, std::string tag) {
   if(!printed){
      std::cerr << tag << " Y" << std::endl;
      std::cerr << mb.Y << std::endl;
   }
}
