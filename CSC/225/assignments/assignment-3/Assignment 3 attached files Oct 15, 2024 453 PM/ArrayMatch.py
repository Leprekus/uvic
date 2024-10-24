#!/usr/bin/env python3
import sys
import math
from typing import List


file = open(sys.argv[1], 'r')
i = 0;
size = -1
a: List[int] = []
b: List[int] = []

# parse input and store num of elements in size, and elements in a and b
for line in file:
    if i == 0:
        size = int(line)

    if i == 1:
        a = [ int(num) for num in line.split(' ') ]

    if i == 2:
        b = [ int(num) for num in line.split(' ') ]

    i += 1

# calculate array size
heap: List[ List[int] ] = [] 

def createSubarray(s: List[int]) -> None:
    if (len(s)/2) % 2 != 0:
        heap.append(s)
        return

    mid = len(s) // 2
    s1 = s[ : mid ]
    s2 = s[ mid :  ]

    createSubarray(s1)
    createSubarray(s2)
    
def case1(s):
    pass
def case2(s):
    pass
def case3(s):
    pass
def match(a: List[int], b: List[int]) -> bool:
    if len(a) % 2 != 0:
        for i, num in enumerate(a):
            if a[i] != b[i]:
                return False
        return True
    createSubarray([*a, *b])
    if case1(heap):
        return True
    if case2(heap):
        return True
    if case3(heap):
        return True
    

    return False
result = 'YES' if match(a, b) else 'NO'
print(result)
