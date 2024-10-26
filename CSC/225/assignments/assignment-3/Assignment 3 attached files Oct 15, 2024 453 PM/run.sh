#!/bin/bash

# Compile the C++ program
g++ ArrayMatch.cpp -o ArrayMatch -Wall -Wextra -Wpedantic -Werror -Wsequence-point -g -DDEBUG -D_GNU_SOURCE -std=c++11 -O0


# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the program with input file
    ./ArrayMatch input000.txt
    
    # Remove the binary after running the program
    rm ArrayMatch
else
    echo "Compilation failed."
fi

