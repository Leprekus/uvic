# partition.py
# CSC 226 - Summer 2025
# 
#
# B. Bird - 2025-05-12

import sys
import itertools
import collections

# Do not change the function name or signature, since we will mark your submission by importing
# this file as a module and calling the partition function.
def partition(A):
    '''
    Determine whether the integer values in A can be partitioned into 
    two lists L and R such that sum(L) == sum(R).

    Parameters:
     A - List of integers
    Returns:
     - If a partition exists, return a pair (L, R) of lists such that every 
       element of A is in exactly one of L or R and sum(L) == sum(R)
     - If no partition exists, return None
    '''
    biggest = max(A)
    total = sum(A)
    k = total / 2
    if (total - biggest) < biggest: return None
    if total % 2: return None

    L = []
    R = [ A.pop() ]
    for r in range(1, len(A) + 1):
        for s in itertools.combinations(A, r):
            if sum(s) == k:
                L = list(s)
                break
    freqA = collections.Counter(A)
    freqL = collections.Counter(L)
    R = R + list( (freqA - freqL).elements() )
    if sum(L) != sum(R):
        return None
    return (L, R)
#partition([20,1,16,2,17,16,8,15,7])
print(partition([x for x in range(0,32)]))


