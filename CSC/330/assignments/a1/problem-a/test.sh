#!/bin/bash

for i in $(seq -w 1 10); do
    echo "Running test $i..."
    runhaskell WordComplete in$i.txt out/out$i.txt

    echo "Comparing output for test $i..."
    if diff -q out$i.txt out/out$i.txt > /dev/null; then
        echo "Test $i: ✅ Passed"
    else
        echo "Test $i: ❌ Failed"
        diff out$i.txt out/out$i.txt
    fi
    echo ""
done

