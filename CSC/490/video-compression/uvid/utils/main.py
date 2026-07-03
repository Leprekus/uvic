from scipy import fft
import numpy as np

'''
In a two-dimensional vector, the elements of axis 0 are rows 
and the elements of axis 1 are columns.
 '''
rows = 0
cols = 1
n = 8
f = lambda x, y: 1/np.sqrt(n) if x == 0 and y == 0 else np.sqrt(2/n)
# Create a dummy data matrix
S = np.arange(64).reshape(8, 8)
# Extract the static matrix (using axis=0)
I = np.eye(n) # identity matrix
C = fft.dct(I, axis=rows, type=2, norm='ortho')

# Compute 2D DCT using matrix multiplication
DCT_2D = C @ S @ C.T

print("input matrix")
print(S)
print("static matrix")
C_str = '{'
for row in C:
    C_str += '\n{'
    for col in row: C_str += str(col) + ','
    C_str += '},'
C_str += '\n}'
print(C_str)
print(C)
D =  C @ S @ C.T
print("coefficient matrix")
print(D)
print("input matrix")
print(np.round(C.T @ D @ C, 1))
