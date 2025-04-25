#!/usr/bin/env python3
import math

array = [17,43,543,23,23,1,23,1,2,3,4,5,345,345,345,3523,1,1,1]
print(array)

def merge(s1, s2, s):
    i = j = k = 0
    while i < len(s1) and j < len(s2):
        if s1[i] <= s2[j]:
            s[k] = s1[i]
            i += 1
        else:
            s[k] = s2[j]
            j += 1
        k+= 1
    while i < len(s1):
        s[k] = s1[i]
        i += 1
        k += 1
    while j < len(s2):
        s[k] = s2[j]
        j += 1
        k += 1
def mergesort(s):
    if len(s) < 2:
        return s
    mid = len(s)//2
    s1 = s[:mid]
    s2 = s[mid:len(s)]
    
    s1 = mergesort(s1)
    s2 = mergesort(s2)
    merge(s1, s2, s)
    return s

mergesort(array)

print(array)
