# For Me
## Compress

Entities at work :
- reader
- stream

**reader**
Handles stream ingestion.

**stream**
Handles processed stream emission.


Things to keep in mind:
- **Keep your data structures tight.** Pass your pixel blocks or residuals 
by reference or use lightweight views/spans to avoid copying memory between 
these files.
- If a stage doesn't need to remember state, it shouldn't be a class. You can 
just use a Namespace with free-standing functions instead of a class.

# Metrics

Prediction Rate:
The total macroblocks written for this video is: 118,800.

Raw linear search lookup over every macroblock in a buffer with 4 frames 
yielded: 88,734 matches in 34.2s

The hash map implementation with the hexagon search yielded
80819 matches in 3.52s

# Final Project

I would like to implement arithmetic coding. GPU parallelization, and focus on performance.
Current bottlenecks:

- poor motion compensation: vector search is trivial
- trivial delta compression: the coefficients are encoded relative to the previous block's values,
compression can be improved by implementing a prediction scheme that further helps reduce delta's based
on information from previous blocks.
