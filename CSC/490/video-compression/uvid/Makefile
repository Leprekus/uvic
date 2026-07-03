EIGEN_PATH= .
EXTRA_CXXFLAGS=
CXXFLAGS=-O3 -Wall -std=c++23 $(EXTRA_CXXFLAGS)
CXXFLAGS+= -I$(EIGEN_PATH)

all: uvid_compress uvid_decompress

uvid_compress: uvid_compress.o
	g++ -o $@ $^

uvid_decompress: uvid_decompress.o
	g++ -o $@ $^



clean:
	rm -f uvid_compress uvid_decompress *.o
