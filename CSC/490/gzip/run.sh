#!/usr/bin/env bash
set -e # exit if a command terminates with non-zero status
set -o pipefail # catch failures in the middle of a pipe

make clean
make all

for entry in ./tests/*; do
	# extract file name from path
	fname=$(basename "$entry")
	# compress
	./main < "$entry" > "./out/$fname.gz" 
	# verify compression is valid
	echo "decompresing: $fname"
	gzip -dc "./out/$fname.gz" > /dev/null 
	echo "success: $fname"
done
#./main < /dev/null > out/main.gz # create empty gzip using main
#echo "=== gzstat.py ===" # validate with gzstat
#python3 gzstat.py --print-block-codes --decode-blocks < out/main.gz
#echo "=== gzip -d ===" # decompress with gzip
#gzip -dc out/main.gz
#echo "=== xxd ===" # view hex output with xxd
#cat out/main.gz | xxd
