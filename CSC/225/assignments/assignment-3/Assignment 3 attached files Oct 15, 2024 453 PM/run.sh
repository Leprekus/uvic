#!/bin/bash

# Compile the C++ program
g++ ArrayMatch.cpp -o ArrayMatch

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the program with input file
    ./ArrayMatch input00.txt
    
    # Remove the binary after running the program
    rm ArrayMatch
else
    echo "Compilation failed."
fi

