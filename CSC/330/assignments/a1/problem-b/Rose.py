from typing import List, Tuple
from collections import deque

problem1 = (6, # number of locations
            [1,1,2,3,1,0], # number of items
            [(1,2,2),(2,3,3),(3,6,4),(1,4,4),(4,3,2),(4,5,3),(5,6,2)]) # edges (start, end, weight)

problem2 = (
        9,
        [1, 1, 1, 1, 1, 1, 1, 1, 1],
        [
            (1, 2, 3),
            (2, 5, 3),
            (1, 6, 2),
            (6, 7, 2),
            (7, 5, 2),
            (5, 3, 1),
            (3, 4, 2),
            (4, 9, 3),
            (5, 8, 2),
            (8, 9, 4)
            ]
        )

type ProblemTuple = Tuple[int, List[int], List[Tuple[int, int, int]]] 
problemsSet: List[ProblemTuple] = []
solutionSet: List[Tuple[int, int] | str] = []
def filename(i): 
    if i < 10:
        return f'0{i}' 
    return f'{i}'
for i in range(1, 11):
    try:
        fIn = open(f'in{filename(i)}.txt')
        numNodes, items, numEdges, *edges = fIn.read().split('\n')
        if int(numEdges) < 1:
            problemsSet.append((
                    int(numNodes),
                    [int(n) for n in items.split()],
                    []))
        else:
            T: ProblemTuple = (
                int(numNodes),
                [int(n) for n in items.split()],
                [
                    (int(a), int(b), int(c))
                    for edge in edges if edge
                    for a, b, c in [edge.split()]
                ]
            )

            problemsSet.append(T)
        fOut = open(f'out{filename(i)}.txt')
        fOutTxt = fOut.read()
        if fOutTxt[0].isdigit():
            minPath, maxItems = fOutTxt.split(' ')
            solutionSet.append((int(minPath), int(maxItems)))
        else:
            solutionSet.append('endpoint inaccessible')

        fIn.close()
        fOut.close()

    except Exception as e:
        raise Exception(f'Failed at {i}: {e}')

type Queue =  List[Tuple[int, int, int]]
def priorityQueue(Q: Queue):
    # ensure tail contains the min
    # sort based on D[i]
    return sorted(Q, reverse=True, key=lambda x: x[1]) 

# takes an ith node 'it' which is the first element in the tuple the  and returns it's index 
def priorityQueueFind(Q: Queue, it: int):
    for  i, t in enumerate(Q):
        if t[0] == it:
            return i
    return -1

def Dijkstra(G, v):
    numNodes, items, edges = G
    if len(edges) < numNodes - 1: return 'endpoint inaccessible'
    # last index should contain shortest distance
    D: List[int] = [float('inf')] * (numNodes + 1)
    D[v] = 0

    # last index should contain maximum number of items
    I: List[int] = [float('-inf')] * (numNodes)
    I[v - 1] = items[v - 1]
    q: Queue = [] # (ith-node, ith-distance, ith-item)
    for i in range (1, numNodes + 1): q.append((i, D[i], items[i - 1]))
    q = priorityQueue(q)

    while q:
        u = q.pop() #retrieve min element (ith-node, ith-weight, ith-item)
        for edge in edges:
            s, e, w = edge # start, end, weight
            # check:
            # if we are exploring nodes adjacent to u
            # and we can relax the edge
            '''
                case 1: D[u] + w < D[z]
                case 2: D[u] + w <= D[z] and has more items
            '''
            if   (u[0] == s and D[s] + w < D[e]) or (u[0] == s and D[s] + w <= D[e] and I[s - 1] + I[e - 1] > I[e - 1]):

                D[e] = D[s] + w 
                # update items carried in this path
                I[e - 1] = I[s - 1] + items[e - 1]
                # update the ith node with the new value
                idx = priorityQueueFind(q, e)
                if idx >= 0: q[idx] = (e, D[e], items[e - 1])
                else: raise Exception(f'Item: {e} not found q: {q}')

            # check if node is adj to z
            elif (u[0] == e and D[e] + w < D[s]) or (u[0] == e and D[e] + w <= D[s] and I[e - 1] + I[s - 1] > I[s - 1]):
                D[s] = D[e] + w
                I[s - 1] = I[e - 1] + items[s - 1]
                idx = priorityQueueFind(q, s)
                if idx >= 0: q[idx] = (s, D[s], items[s - 1])
                else: raise Exception(f'Item: {s} not found')

            q = priorityQueue(q)
    #assert numNodes == len(res) # return impossible if this fails
    if D[len(D) - 1] == float('inf'): return 'endpoint inaccessible'
    return [D[ len(D) - 1 ], I[ len(I) - 1 ]]


#print('Dijkstra',Dijkstra(problemsSet[9], 1))
def test():
    for i, p in enumerate(problemsSet):

        
        res = Dijkstra(p, 1)
        sol = solutionSet[i]
        if isinstance(sol, str) and res == sol:
            print(f'test {i + 1}: passed')
        elif res[0] == sol[0] and res[1] == sol[1]:
            print(f'test {i + 1}: passed')
        else: print(f'test {i + 1}: failed expected: {sol} got: {res}')

test()
