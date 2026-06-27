Original deflate:

1. writes gzip container header
2. processes bitstream
3. writes crc & uncompressed size

# Development

The architecture initially attempted to run on constrained resources.
Since the required buffer size by the documentation is 32k, the idea
was to have the compressor run solely relying on 32k bytes of stack memory.
This proved challenging and the idea was abandoned as further blocks were implemented.

# Architecture

The compressor uses function pointers for the stream handlers. These enable
the end user to use the software as a library while remaining implementation
independent.

The pipeline follows a multi-stage buffer as follows:
 stdin ingestion -> 32K stream buffer -> emit-handler -> io-buffer
 The processing occurs at intermediate steps between the 32k stream buffer 
 and the emit-handler where the stream gets deflated. 
 emit-handler gets the processed information delivered.
 The io-buffer stores 4k bytes and flushes whenever it is full or at the end of
 the stream.

deflate reads into half of the buffer (first 16k bytes) then attempts to make
another read. If the second half is full processing continues without setting 
the FINISH flag. otherwise, the FINISH flag is set and the last byte gets padded.

This process continues filling up half of the buffer, processing it and then 
filling up the other half until all the input has been read.

# Data Structures

In order to emit backreferences, a map is used for keys 0..255. The current
stream (16k) gets stored as an intermediate representation of Literals
and Length, Distance pairs, and is then sorted. The map stores the first entry 
to each key in the array. Additionally, we have the number of items starting
with each key, so the array gets essentially partitioned into buckets. The
buckets are created finding all of the prefixes in the string, they are
then sorted by key and index. Meaning that the strings 
(1) abcdef and (2) abcgraph come sequentially.
 This allows the algorithm to produce backreferences
by finding itself and then finding the longest match looking backwards using
binary search. The algorithm was working during the development of block type1,
due to time constraints it remains unfixed.


# References

https://create.stephan-brumme.com/length-limited-prefix-codes/

https://github.com/richgel999/sserangecoding/blob/main/packagemerge.h
