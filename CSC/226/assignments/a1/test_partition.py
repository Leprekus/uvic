# test_partition.py
# CSC 226 - Summer 2025
# 
# You can use this program to test your implementation of the partition() function
# in partition.py. We will use a similar protocol to mark your submission.
#
# This program reads a sequence of integer values from standard input (i.e. the user)
# and, if a partition is returned, verifies that the partition is correct.
#
# To enter values manually, run this program with the command 
#   python test_partition.py 
# and enter a sequence of integers separated by spaces, then press Ctrl-D to terminate input.
#
# To read values from a text file, use a command like
#   python test_partition.py < file_with_values.txt
#
# B. Bird - 2025-05-12

import sys 
import partition

if __name__ == '__main__':
    input_values = [int(s) for s in sys.stdin.read().split()]
    print('Input values:',' '.join(f'{i}' for i in input_values))
    result = partition.partition(input_values.copy())
    if result is None:
        print("No partition exists")
    else:
        L, R = result
        print('L:',' '.join(f'{i}' for i in L))
        print('R:',' '.join(f'{i}' for i in R))
        print("Sum of L:",sum(L))
        print("Sum of R:",sum(R))
        if sorted(input_values) != sorted(list(L) + list(R)):
            print("Error: Contents of L and R do not match the input values")
        if sum(L) != sum(R):
            print("Error: sum(L) != sum(R)")
