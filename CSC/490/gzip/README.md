Original deflate:

1. writes gzip container header
2. processes bitstream
3. writes crc & uncompressed size

# 2. Processing bitstream

The sliding window's length is read into a buffer. Its length "lookahead" is 
either 32,768 or 65,536 depending on the machine's int size.

Deflate computes profitability on flushing the block at the current point
based on whether the buffer is full or the distance`trees.c ct_tally()`.

When a block is flushed a distance tree is constructed. If we are unable
to compress we flush without a header. Else if we meet some weird condition
we flush with a header. Finally, the buffer contents are emitted.
`trees.c flush_block()`

# Naming Conventions
- sqlite3_prefix_snake: public facing API
- snake_case_no_prefix: internal API
- CamelCase: Data types & struct names
- sqlite3Camel: ???
