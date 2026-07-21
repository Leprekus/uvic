EIGEN_PATH= .
EXTRA_CXXFLAGS=
CXXFLAGS=-O3 -Wall -std=c++23 $(EXTRA_CXXFLAGS)
CXXFLAGS+= -I$(EIGEN_PATH)
CXX= g++-16
all: uvid_compress uvid_decompress
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
uvid_compress: uvid_compress.o utils.o
	$(CXX) -o $@ $^

uvid_decompress: uvid_decompress.o utils.o
	$(CXX) -o $@ $^

sandbox: sandbox.o utils.o
	$(CXX) -o $@ $^


clean:
	rm -f uvid_compress uvid_decompress *.o
