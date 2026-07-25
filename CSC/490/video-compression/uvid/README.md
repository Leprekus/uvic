# Documentation

## Overview 

The techniques used to achieve video compresion for this assignment include:
DCT transformation, Quantization, Vector Search, Motion Compensation, P-Frames, and Delta compression.

The DCT transformation and Quantization take a 16x16 macroblock for the Y plane or an 8x8 block for the Cb and Cr planes. The DCT transformation and quantization matrices are hard coded for both the 16x16 and 8x8 blocks.

The compressor and decompressor buffer the four most current frames. These are used to predict entire frames, as well as to perform inter-frame prediction. Inter frame prediction Macroblock &mb, auto i, auto x, auto y takes as parameters the index of the current frame (relative to the number of frames currently in the buffer), and the x and y positions. It picks the best match based on the AAD score and looks for macroblocks in the top, left, right and top-left diagonal. 

Each compressed frame is written to the bitstream along with a six byte motion vector tthat contains the x, y coordinates of the referenced frame, the i index of its position in the buffer, and an is_copy flag.
The vectors are created during intra and inter prediction.

Inter prediction  uses the current block's x, y coordinates to grab the block in the previous buffered frame as a starting position. Then a traversal along the buffered frames is performed using the same x, y coordinates. The best starting point is picked using a sum of absolute differences (SAD) where the tolerance level is a difference of ~1.39 per pixel. If the value is less than or equal to the tolerance level, the frame is flagged as a copy. The compressor only outputs a vector for this frame, the decompressor retrieves the matching block from the buffer and writes it to cout. Frames that are not copeies, that is SAD > tolerance,we take the difference between the current frame and the best matching reconstructed frame. DCT and quantization are then performed before writing to stream.

The motion compensation for interpredicted frames is trivial, matching blocks are searched for in an L-shape pattern along the diagonals, and it looks at the top and bottom frame. 

# Features Implemented

- Documentation
- P-Frames
- Motion Compensation
- Real time decompression
# Notes on Improvment
I would like to implement arithmetic coding. GPU parallelization, and focus on performance.
Current bottlenecks:

- poor motion compensation: vector search is trivial
- trivial delta compression: the coefficients are encoded relative to the previous block's values,
compression can be improved by implementing a prediction scheme that further helps reduce delta's based
on information from previous blocks.
