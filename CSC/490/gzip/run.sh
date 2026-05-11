#!/usr/bin/env bash
make clean
make all
./main > tests/s.gz
echo "=== gzstat.py ==="
python3 gzstat.py --print-block-codes --decode-blocks < tests/s.gz
echo "=== xxd ==="
cat tests/s.gz | xxd
