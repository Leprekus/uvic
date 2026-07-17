#include "utils.hpp"
void intra_reconstruct(
      std::optional<FrameBuffer> &buffer, Quality qual,
      Macroblock &mb, 
      int i, int x0, int y0,
      std::pair<i8, i8> offset) {
   auto [x, y] = offset;
   if(x == y && y == -1) {
      iframe_inverse(qual, mb);
      return;
   };
   Macroblock &decompressed = buffer->get_frame_mb(i, x0 + x, y0 + y);
   predicted_inverse(mb, decompressed);

}

extern bool printed;
void print(const Macroblock &mb, std::string tag) {
   if(!printed){
      std::cerr << tag << " Y" << std::endl;
      std::cerr << mb.Y << std::endl;
      std::cerr << "vector" << std::endl;
      std::cerr << static_cast<int>(mb.vect.first) << " " << static_cast<int>(mb.vect.second) << std::endl;
   }
}
