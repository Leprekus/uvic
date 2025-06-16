Although I failed to implement the algorithm in Haskell, I came up with an almost fully working solution in Python which the goal was to write in haskell.

## Intuition

Since we are trying to find single source shortest path, I decided to use Dijkstra's algorithm. After analyzing the problem I realized that the requirements create a sort of waterfall like dependency. Where it is required to first find the length of the shortest path, so we can then find the shortest path with the maximum number of items.

### Cost Function
Dijkstra has a cost function (D[u] + w((u,v)) < D[v]) which we are trying to minimize and compute at the same time. My strategy was to take that cost function and modify it so we minimized the path, while at the same time maximize the items that we gathered. The result of that is:
(D[u] + w((u,v) < D[v]) or (D[u] + w((u, v)) <= D[v] and I[u - 1] + I[v - 1] >= I[e - 1]))
The logic behind it is as follows:

### Breaking down the cases
Case 1: we find a path shorter than the current one in which case the LHS of the or is true (this is regular Dijkstra's cost function)
Case 2: we find a path less than or equal to the one we already computed in which case we cannot get any worse than what we have already computed, but we can maximize our items. So by checking if the items following this path are more than or equal to the ones we had previously computed (again greater than or equal because we are trying to maximize items so we can't be any worse than we currently are) we should stick to the new path. The reason why the Python code checks for strict '>' when it comes to the items is that there are some cases where I pop items from the queue early so when the RHS of the 'or' predicate evaluates to True these items are no longer in the queue which result in an exception raised. 

### Running the Test cases on the Python File
Executing ```python3 Rose.py``` will load all test files (1..10), run the input through Dijkstra and compare the result with its corresponding out file.
