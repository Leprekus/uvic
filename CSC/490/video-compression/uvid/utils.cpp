#include "utils.hpp"

/* uses relative offsets w.r.t current block */
void intra_reconstruct(
      std::optional<FrameBuffer> &buffer, Quality qual,
      Macroblock &mb, 
      int i, int x0, int y0,
      BlockVect offset) {
   auto [x, y, z] = offset;
   if(x == y && y == -1) {
      iframe_inverse(qual, mb);
      return;
   };
   if(z == -1) z = buffer->frame_count();
   Macroblock &decompressed = buffer->get_frame_mb(z, x0 + x, y0 + y);
   predicted_inverse(mb, decompressed);

}
/* uses absolute offsets w.r.t current block */
void inter_reconstruct(
      std::optional<FrameBuffer> &buffer, Quality qual,
      Macroblock &mb, 
      int i, int x0, int y0,
      BlockVect offset) {
   auto [x, y, z] = offset;
   if(x == y && y == -1) {
      iframe_inverse(qual, mb);
      return;
   };
   Macroblock &decompressed = buffer->get_frame_mb(z, x, y);
   predicted_inverse(mb, decompressed);

}

extern bool printed;
void print(const Macroblock &mb, std::string tag) {
   if(!printed){
      std::cerr << tag << " Y" << std::endl;
      std::cerr << mb.Y << std::endl;
      std::cerr << "vector" << std::endl;
      std::cerr << static_cast<int>(std::get<0>(mb.vect)) << " " << static_cast<int>(std::get<1>(mb.vect)) << " " << static_cast<int>(std::get<2>(mb.vect)) << std::endl;
   }
}
/*
 * 00         - end of stream
 * 01         - 1+ repetitions
 * 01 00      - run of 1
 * 01 10      - run of 2
 * 01 110   1 - run of 3  
 * 01 1110  00 - run of 4  
 * 01 1110  10 - run of 6
 * 01 11110 000 - run of 8
 * RUN LENGTH   VALUE
 * */
void write_zero(u32 run, OutputBitStream &stream){
   // push zero for no delta
   stream.push_bit(0); //std::cerr << "0";
   if(run) {
      stream.push_bit(1); //std::cerr << "1"; // push one for at least one repetition 
      if(run == 1) { // handle a run of 1
         stream.push_bit(0); //std::cerr << "0";
         stream.push_bit(0); //std::cerr << "0";
      } else if(run == 2) { // handle a run of 2
         stream.push_bit(1); //std::cerr << "1";
         stream.push_bit(0); //std::cerr << "0";
      } else { // handle a run of 3+
         // output length in unary
         int size = std::bit_width(run);
         for(int i = 0; i < size; i++) {
            stream.push_bit(1); 
            //std::cerr << "1"; 
         }
         // end of unary length
         stream.push_bit(0); //std::cerr << "0";

         // output value except MSB  
         assert(size >= 2);
         for(int i = size - 2; i >= 0; i--) {
            u32 bit = (run>>i) & 1U;
            stream.push_bit(bit); 
            //std::cerr << (bit == 1 ? "1" : "0");
         }

      }
   } else { // push zero for no repetitions (end of stream)
      stream.push_bit(0); //std::cerr << "0";
   }
   //std::cerr << "\n";

}
/*
 * 11         - there is a delta & 1+ reps
 * 11 0        - (- delta)
 * 11 1        - (+ delta) 
 * 11 x xxx yyy - size and value of coefficient 
 * 11 x xxx xxx ccc ddd - size and value of reps
 * */
void write_coefficient(int value, u32 run, OutputBitStream &stream){
   for(int count = 0; count < run; count++) { 
      assert(run && value != 0);
      u32 abs_val = static_cast<u32>((value >= 0) ? value : -value);
      // push 1 for delta
      stream.push_bit(1); //std::cerr << "1";

      // direction of delta
      if(value >= 0) { // push one if value is positive
         stream.push_bit(1); //std::cerr << "1";
      } else {  // push zero if value is negative
         stream.push_bit(0); //std::cerr << "0";
      }
      // value of coefficient
      if(abs_val == 1) {
         stream.push_bit(0); //std::cerr << "0";
         stream.push_bit(0); //std::cerr << "0";
      }  else if(abs_val == 2) {
         stream.push_bit(1); //std::cerr << "1";
         stream.push_bit(0); //std::cerr << "0";
      } else {
         // output length in unary
         int size = std::bit_width(abs_val);
         for(int i = 0; i < size; i++) {
            stream.push_bit(1); 
            //std::cerr << "1"; 
         }
         stream.push_bit(0); //std::cerr << "0"; 

         // output coefficient except for MSB
         assert(size >= 2);
         for(int i = size - 2; i >= 0; i--) {
            u32 bit = (abs_val>>i) & 1U;
            stream.push_bit(bit); 
            //std::cerr << (bit == 1 ? "1" : "0");
         }
      }
   }
   
}
void write_bitstream(const auto value, const u32 run, OutputBitStream &stream) {
   bool value_is_zero = value == 0;
   if(value_is_zero) {
      write_zero(run, stream); 
   } else {
      write_coefficient(value, run, stream);
   }
   
}

std::pair<u32, u32> read_zeroes(InputBitStream &stream) {
   u32 msb = (stream.read_bit() << 1) | stream.read_bit();
   if(msb == 0b00) {
      return std::pair(0, 1);
   }
   if(msb == 0b10) {
      return std::pair(0, 2);
   }
   msb = 2;
   while(stream.read_bit()) msb++;

   assert(msb >= 2);
   u32 run = 1U << (msb - 1);
   for(int i = msb - 2; i >= 0; i--)
      run |= (stream.read_bit() << i);
   return std::pair(0, run);
}
int read_coefficient(InputBitStream &stream, u32 dir) {
   assert(dir == 2U || dir == 3U);
   int sign = 0;
   if(dir == 0b10) sign = -1; 
   else if (dir == 0b11) sign = 1; 
   else std::runtime_error("expected a value between 0b10 and 0b11");

   u32 msb = (stream.read_bit() << 1) | stream.read_bit();
   if(msb == 0b00) return 1 * sign; 
   if(msb == 0b10) return 2 * sign;
   u32 size = 2;
   while(stream.read_bit()) size++;
   assert(size >= 2);
   u32 coeff = 1U << (size - 1);
   for(int i = size - 2; i >= 0; i--)
      coeff |= (stream.read_bit() << i);

   
   return static_cast<int>(coeff) * sign;
}



template <typename Derived>
void _bitstream_to_compressed_block(Eigen::MatrixBase<Derived> &M, InputBitStream &stream, const auto &traversal) {

   assert(M.cols() == M.rows());
   assert(M.cols() * M.cols() == traversal.size());

   u32 curr = (stream.read_bit() << 1) | stream.read_bit();
   int idx = 0;
   while(curr) {
      if(curr == 0b01) {
         auto [val, run] = read_zeroes(stream);
         for(int i = 0; i < run; i++) {
            auto [x, y] = traversal[idx++];
            M(x, y) = val;
         }
      } else if(curr >= 0b10) {
         int coeff = read_coefficient(stream, curr);
         auto [x, y] = traversal[idx++];
         M(x, y) = coeff;
      }
      else throw std::runtime_error("expected a value between 1 and 3");
      curr = (stream.read_bit() << 1) | stream.read_bit();
   }
}
void bitstream_to_compressed_mb(Macroblock &mb, InputBitStream &stream) {
   _bitstream_to_compressed_block(mb.Y,  stream, zigzag_scan_16x16);
   _bitstream_to_compressed_block(mb.Cb, stream, zigzag_scan_8x8);
   _bitstream_to_compressed_block(mb.Cr, stream, zigzag_scan_8x8);
}

/* get run q*/
template <typename EigenMatrix>
void _compressed_block_to_bitstream(OutputBitStream &stream, const EigenMatrix &M,  const auto &traversal) {
   assert(M.cols() == M.rows());
   assert(M.cols() * M.cols() == traversal.size());

   auto curr = M(traversal.front().first, traversal.front().second);
   u32 run = 0;
   for(auto [x, y]: traversal) {
      if(curr == M(x,y)) {
         run++;
      } else {
         write_bitstream(curr, run, stream);
         curr = M(x, y);
         run = 1;
      }
   }
   if(run) {
      write_bitstream(curr, run, stream);
   }

   // mark end of MB
   stream.push_bit(0);
   stream.push_bit(0);
   
}
void compressed_mb_to_bitstream(const Macroblock &mb, OutputBitStream &stream) {
   
   //TODO: compare results with zigzag, row & col scans vs pure zigzag
   _compressed_block_to_bitstream(stream, mb.Y, zigzag_scan_16x16);
   _compressed_block_to_bitstream(stream, mb.Cb, zigzag_scan_8x8);
   _compressed_block_to_bitstream(stream, mb.Cr, zigzag_scan_8x8);
}
