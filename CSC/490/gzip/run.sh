#!/usr/bin/env bash
make clean
make all
./main < /dev/null > out/main.gz # create empty gzip using main
echo "=== gzstat.py ===" # validate with gzstat
python3 gzstat.py --print-block-codes --decode-blocks < out/main.gz
echo "=== gzip -d ===" # decompress with gzip
gzip -dc out/main.gz
echo "=== xxd ===" # view hex output with xxd
cat out/main.gz | xxd
