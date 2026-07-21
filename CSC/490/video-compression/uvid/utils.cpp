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

void write_zero(const int run, OutputBitStream &stream){
   // push zero for no delta
   stream.push_bit(0);
   if(run) {
      stream.push_bit(1); // push one for at least one repetition 
      if(run == 1) { // handle a run of 1
         stream.push_bit(0);
         stream.push_bit(0);
      } else if(run == 2) { // handle a run of 2
         stream.push_bit(1);
         stream.push_bit(0);
      } else { // handle a run of 3+
         int tmp = run; // get bits count
         while(tmp > 0) { tmp >>= 1; stream.push_bit(1); }
         stream.push_bit(0);
      }
   } else { // push zero for no repetitions (end of stream)
      stream.push_bit(0);
   }
   
}
void write_coefficient(const double value, u32 run, OutputBitStream &stream){}
void write_bitsream(const double value, u32 run, OutputBitStream &stream) {
   bool value_is_zero = value == 0;
   
   if(value_is_zero)
      write_zero(run, stream); 
   else 
      write_coefficient(value, run, stream);
   
}

void read_zeroes(InputBitStream &stream) {
   int should_continue = stream.read_bit();
   if(!should_continue) return; // end of stream
   int run = stream.read_bits(2);
   if(run == 0); // run of 1
   else if(run == 2); // run of 2
   else {
      int len = 2;
      while(stream.read_bit()) { len++; }
      u32 run = 0;
      while(--len) { run |= (stream.read_bit() << len); }
   }
}
void read_coefficient(InputBitStream &stream) {
}

void read_bitstream(InputBitStream &stream){
   u8 value_is_zero = stream.read_bit();
   if(value_is_zero) read_zeroes(stream);
   else read_coefficient(stream);
   
}

template <typename EigenMatrix>
void _decompress_block(const EigenMatrix &M, InputBitStream &stream) {

}
void decompress_mb(const Macroblock &mb, InputBitStream &stream) {
   _decompress_block(mb.Y,  stream);
   _decompress_block(mb.Cb, stream);
   _decompress_block(mb.Cr, stream);
}

template <typename EigenMatrix>
void _compress_block(const EigenMatrix &M, OutputBitStream &stream) {
   assert(M.cols() == M.rows());
   size_t len = M.size();
   size_t cols = M.cols();

   
}
void compress_mb(const Macroblock &mb, OutputBitStream &stream) {
   _compress_block(mb.Y,  stream);
   //_compress_block(mb.Cb, stream);
   //_compress_block(mb.Cr, stream);
}
