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
void _bitstream_to_compressed_block(const EigenMatrix &M, InputBitStream &stream) {

}
void bitstream_to_compressed_mb(const Macroblock &mb, InputBitStream &stream) {
   _bitstream_to_compressed_block(mb.Y,  stream);
   _bitstream_to_compressed_block(mb.Cb, stream);
   _bitstream_to_compressed_block(mb.Cr, stream);
}

/* get run q*/
template <typename EigenMatrix>
void _compressed_block_to_bistream(OutputBitStream &stream, const EigenMatrix &M,  auto &traversal) {
   assert(M.cols() == M.rows());
   assert(M.cols() * M.cols() == traversal.size());
   size_t len = M.size();
   size_t cols = M.cols();

   auto curr = M(traversal.front().first, traversal.front().second);
   int run = 0;
   for(auto [x, y]: traversal) {
      if(curr == M(x, y))
         run++;
      else {
         std::cerr << "item: " << curr << " run: " << run << "\n";
         run = 1;
         curr = M(x, y);
      }
   }
   if(run) {
      std::cerr << "item: " << curr << " run: " << run << "\n";
   }

   
}
void compressed_mb_to_bistream(const Macroblock &mb, OutputBitStream &stream) {
   auto [x, y, idx] = mb.vect;
   bool row_scan = x != 0 && y == 0;
   bool col_scan = x == 0 && y != 0;
   bool zig_scan = x != 0 && y != 0;

   auto *ptr_8x8   = &row_scan_8x8;
   auto *ptr_16x16 = &row_scan_16x16;
   if(row_scan){} // default so ignore
   if(col_scan) {
      ptr_8x8   = &col_scan_8x8;
      ptr_16x16 = &col_scan_16x16;
   }
   if(zig_scan) {
      ptr_8x8 = &zigzag_scan_8x8;
      ptr_16x16 = &zigzag_scan_16x16;
   }
   auto &ref_8x8 = *ptr_8x8;
   auto &ref_16x16 = *ptr_16x16;
   std::cerr << "col_scan: " << col_scan  << " row " << row_scan << " col " << col_scan << " zig " << zig_scan << "\n";
   //_compressed_block_to_bistream(stream, mb.Y, ref_16x16);
   _compressed_block_to_bistream(stream, mb.Cb, ref_8x8);
   //_compress_block(mb.Cr, stream);
}
