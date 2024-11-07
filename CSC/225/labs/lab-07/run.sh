#!/bin/bash

# Loop through each .txt file in the current directory
for file in *.txt; do
    echo "Output for $file:"
    # Run ./a.out with the file as input and display the output
    ./a.out "$file"
    echo # Add an empty line for readability
done

