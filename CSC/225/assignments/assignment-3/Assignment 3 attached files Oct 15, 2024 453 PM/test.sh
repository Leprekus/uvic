#!/bin/bash

# Array of input files and expected results
inputs=("input00.txt" "input01.txt" "input02.txt" "input03.txt" "input04.txt" "input05.txt")
expected=("YES" "YES" "YES" "NO" "NO" "YES")

# Iterate over the inputs and compare outputs
for i in "${!inputs[@]}"; do
    input_file="${inputs[i]}"
    expected_result="${expected[i]}"
    
    # Run the program with the input file and capture the output
    output=$(./ArrayMatch "$input_file")

    # Compare the output with the expected result
    if [ "$output" == "$expected_result" ]; then
        echo "$input_file -> Correct (Expected: $expected_result, Got: $output)"
    else
        echo "$input_file -> Incorrect (Expected: $expected_result, Got: $output)"
    fi
done

