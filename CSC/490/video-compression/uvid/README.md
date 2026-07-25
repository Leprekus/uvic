# Documentation

## Overview 

The techniques used to achieve video compresion for this assignment include:
DCT transformation, Quantization, Vector Search, Motion Compensation, P-Frames, and Delta compression.

The DCT transformation and Quantization take a 16x16 macroblock for the Y plane or an 8x8 block for the Cb and Cr planes. The DCT transformation and quantization matrices are hard coded for both the 16x16 and 8x8 blocks.

The compressor and decompressor buffer the four most recent frames. The buffer is used to predict frames below a tolerance threshold, and to perform inter-frame prediction. Inter frame prediction takes as parameters the index of the current frame (relative to the number of frames currently buffered), and the x, y coordinates of the macroblock. It picks the best match based on the AAD score and looks for macroblocks in the top, left, right and top-left diagonal of the current frame being searched.. 

Each compressed frame is written to the bitstream along with a six byte motion vector tthat contains the x, y coordinates of the referenced frame, the i index of the frame relative to its position in the buffer, and an is_copy flag. When the is_copy flag is set, only the vector is transmitted, and the decompressor reuses a macroblock from the referenced frame that is currently in the buffer.

Inter prediction  uses the current block's x, y coordinates to grab the block in the previous buffered frame as a starting position. Then a traversal along the buffered frames is performed using the same x, y coordinates. The best starting point is picked using a sum of absolute differences (SAD) where the tolerance level is a difference of ~1.39 per pixel. If the value is less than or equal to the tolerance level, the frame is flagged as a copy. The compressor only outputs a vector for this frame, the decompressor retrieves the matching block from the buffer and writes it to cout. Frames that are not copies, that is SAD > tolerance, we take the difference between the current frame and the best matching reconstructed frame and emit the deltas.. DCT and quantization are then performed. Finally, the bistream is performed using RLE before writing to stream.

The motion compensation for interpredicted frames is trivial, matching blocks are searched for in an L-shape pattern along the diagonals and the top and bottom blocks are also searched.

# Features Implemented

- Documentation
- P-Frames
- Motion Compensation
- Real time decompression

## Architecture

The most important `struct` is `Macroblock`
Which consists of a 16x16 (Y), 8x8 (Cb), and 8x8 (Cr) eigen matrices. 

Compressor:
Consists of a three stage pipeline.
Reading from stream:
Reads 384 bytes to fill a macroblock consisting of the 16x16 Y frame, 8x8 Cb frame, and 8x8 Cr frame. 

processing and buffering:
Once a Macroblock has been read, it is processed. If it is the first frame in the buffer, it gets encoded as an i-frame. If there is a good match intra-encoding is performed,the compressed result gets stored in a `compressed` bufer, and the decompressed result gets stored in a `decompressed` buffer.
Following frames 2, 3, and 4 get encoded as P-frames. If there are good matches the transformed (DCT'd and Quantized) deltas are stored. If there are excellent matches, an is_copy flag is set and only the vector is transmitted.  

encoding and writing to stream:
Once the buffer has read four frames, we iterate over the buffer starting from frame one and write each macroblock into the bitstream. The write subsystem performs RLE and emits the compressed 0 runs and coefficients. 

decompressor:
Consists of a three stage pipeline.

Read and Decompress:
Reads the RLE run and writes the decompressed stream into a Macroblock.

Decode and Buffer:
The Macroblock gets treated as an I-Frame if it is in the first frame. If there is a motion vector, the match is looked up in the buffer and the block is reoconstructed using the transmitted deltas. P-frames are reconstructed in the same fashion, but the vector includes an index, to know which frame in the buffer to reference to. Copies are looked up in the same fashion. When a vector with the copy flag is read, we skip reading data from the bistream into the macroblock and look up directly in the buffered frames.

Write:
Once the buffer is full all frames are written from the buffered in a FIFO fashion.
## Bitstream

Header:
9 bytes:
- 4 bytes for the unsigned width integer
- 4 bytes for unsigned height integer
- 1 byte to indicate quality setting:
that can take any of the following values:
0 - low
1 - medium
2 - high

Body:
Each frame begins with a 1 byte flag set to 1.
The end of the bitstring for each frame is padded to a byte.
Once all frames have been finished processing a 1 byte flag set to 0 is emitted.

The bistream is encoded MSB first. To read the number two in binary, one would need to read_bit() << 1 | read_bit().
The first two bits are reserved to indicate how to parse the bitstring:
00 - end of stream
01 - run of 1+ zeroes
10 - negative coefficient
11 - positive coefficient.

The run of zeroes and coefficients are unary-encoded. To read a zero run, the length N of the coefficient in unary must be read, and then N-1 subsequent bits. The MSB is implicitly set. To read a run of 12 zeroes one would read the following bitstring:
 01               11110                100 
 ^ run of zeroes  ^ unary length of 12 ^ value of 12 with MSB implicitly set
Which means repeat a zero 12 times.
After this bistring one would expect to read next either: end of stream, run of zeroes, or a coefficient.
 The coefficients are encoded in the same fashion, the only thing that changes are the first two bits to indicate whether the coefficient is positive or negative.

 

## Bilbiograpy

[1] G. Sullivan, “Overview of H.264/MPEG-4 Part 10,” in Visual Communications and Image Processing 2005, vol. 5960, C. W. Chen, D. Zhou, and H.-M. Hang, Eds. SPIE, 2005, Art. no. 10.1117/12.633506.

[2] T. Wiegand, G. J. Sullivan, G. Bjøntegaard, and A. Luthra, “Overview of the H.264/AVC video coding standard,” IEEE Transactions on Circuits and Systems for Video Technology, vol. 13, no. 7, pp. 560–576, Jul. 2003.

[3] ScienceDirect Topics, “Intra Prediction,” Elsevier. [Online]. Available: https://www.sciencedirect.com/topics/computer-science/intra-prediction. [Accessed: Jul. 24, 2026].

[4] S. Gupta and A. Sharma, “VLSI implementation of intra prediction for H.264/AVC video codec,” IOSR Journal of VLSI and Signal Processing (IOSR-JVSP), vol. 1, no. 6, pp. 10–18, 2013.

[5] MIT Computer Science and Artificial Intelligence Laboratory (CSAIL), “Group 3 project report: Hardware implementation of H.264 intra prediction,” 6.375 Complex Digital Systems, MIT, Cambridge, MA, USA, Tech. Rep., 2006.

[6] Wikipedia, “JPEG: Discrete cosine transform,” Wikipedia, The Free Encyclopedia. [Online]. Available: https://en.wikipedia.org/wiki/JPEG#Discrete_cosine_transform. [Accessed: Jul. 24, 2026].

[7] NVIDIA Corporation, “8x8 Discrete Cosine Transform (DCT) implementation,” NVIDIA CUDA Documentation/Whitepaper, Tech. Rep., 2008.

# Notes For self on Improvment
I would like to implement arithmetic coding. GPU parallelization, and focus on performance.
Current bottlenecks:

- poor motion compensation: vector search is trivial
- trivial delta compression: the coefficients are encoded relative to the previous block's values,
compression can be improved by storing frame as pixels instead of macroblocks.
compression can be achieved by using huffman per-frame after RLE.
