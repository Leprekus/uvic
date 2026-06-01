#!/usr/bin/env bash
set -e
make all
fname=$(basename "$1")
./main < $1 > "./out/$fname.gz" # create empty gzip using main
echo "=== gzstat.py ===" # validate with gzstat
python3 gzstat.py --print-block-codes --decode-blocks < "./out/$fname.gz"
echo "=== gzip -d ===" # decompress with gzip
gzip -l "./out/$fname.gz"  
gzip -dc "./out/$fname.gz"
echo "=== xxd ===" # view hex output with xxd
cat "./out/$fname.gz" | xxd
