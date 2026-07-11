import io
import numpy as np
from scipy import fft, linalg


'''
In a two-dimensional vector, the elements of axis 0 are rows 
and the elements of axis 1 are columns.
 '''
rows = 0
cols = 1
n = 8

alpha = lambda x: 1/np.sqrt(n) if x == 0 else np.sqrt(2/n)
cos = lambda u, x: np.cos( (np.pi*(2*x+1)*u)/(2*n) )
f = np.eye(n)
C = np.zeros((n, n))
for i in range(n):
    for j in range(n):
        if i == 0:
                C[i][j] = np.sqrt(1/8);
        else:
                C[i][j] = (np.sqrt(1/4)) * np.cos(((2*j + 1) * i * np.pi)/(16));
        
'''
C = fft.dct(np.eye(n),  type=2, axis=0, norm='ortho')
# Create a dummy data matrix
'''
S = np.arange(64).reshape(8, 8)
# Extract the static matrix (using axis=0)
#for u in range(n):
#    for v in range(n):
#        DCT[u][v] *= (alpha(u) * alpha(v))
#fft.dct(DCT, axis=cols, type=2, norm='ortho')
print("static matrix")
C_str = '{'
for row in C:
    C_str += '\n{'
    for col in row: C_str += str(col) + ','
    C_str += '},'
C_str += '\n}'
#print(C_str)
#print("S matrix")
#print(S)
#print("Reconstructed S matrix")
#T = C.T @ (C @ S @ C.T) @ C
#print(T.round())

raw_data = \
'''
 1  1 -1 -1 -1  1  1  0 -1 -1  1  1  1  1  1  0
-1  1  0 -1  0  1  1  0 -1 -1  1  1  1  1  0  0
 0  0 -1 -1  0  1  1  0 -1 -1  1  1  1  1  0  1
 1  0 -1 -1 -1  1  1  0 -1 -1  1  1  1  1  0  1
 1  0 -1 -1 -1  1  1  1 -1  0  1  1  1  1  0  0
 1  0 -1 -1  0  1  1  1 -1 -1  1  1  1  1  0  1
 1  0 -1 -1  1  1  0 -1 -1 -1 -1 -1 -1 -1 -1 -1
 1  1  0 -1  0  1  0 -1 -1  1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1  1  0 -1  1  1  1  1  1  1  1  1  1
-1 -1 -1 -1  1  0  0  1  1  0  1  1  1  1  0  1
-1 -1 -1 -1  0  0  1  1 -1  0  1  1  1 -1 -1  1
-1 -1 -1 -1  0 -1  1  1 -1 -1 -1  0  0 -1 -1  1
-1 -1 -1 -1 -1 -1  1  1 -1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1  1  1 -1 -1 -1 -1 -1 -1 -1 -1
-1  0 -1 -1 -1  0  1  1 -1 -1  1  1  1 -1 -1  0
-1 -1 -1 -1 -1  0  1  1 -1  0  1  1  1  0 -1  1
'''
M = np.loadtxt(io.StringIO(raw_data.strip()), dtype=int)
H = linalg.hadamard(16)
T = H @ M @ H
print('original')
print(M)
print('transformed')
print(T)
print('pre scaled')
print(H @ T @ H)
print('scaled')
print((H @ T @ H)/(16*16))
