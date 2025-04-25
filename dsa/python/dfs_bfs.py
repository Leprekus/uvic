
from typing import List


def bfs( M: List[ List[int] ] ):
    stack = [ M[0][0] ]
    visited = [ M[0][0] ]
    while len( stack ) > 0:
        curr = stack.pop()
        for n in M[ curr ]:
            if not n in visited:

            
