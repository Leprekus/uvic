import Data.List (sortBy)

problem1 = (6, -- number of locations
            [1,1,2,3,1,0], -- num items
            [(1,2,2),(2,3,3),(3,6,4),(1,4,4),(4,3,2),(4,5,3),(5,6,2)]) -- edges

queue :: Ord a => [a] -> [a]
queue [] = []
queue a = sortBy compare a

listFrom :: Integer -> a -> [a]
listFrom 0 _ = []
listFrom n value = [value] ++ listFrom (n-1) value

len = toInteger . length

(nodes, items, edges) = problem1 
numEdges = len edges


getAdj :: (Integer, Integer) -> [(Integer, Integer, Integer)] -> [(Integer, Integer, Integer)]
getAdj _ [] = []
getAdj (u, w) ((start, end, weight):es)
    | u == start = (start, end, weight):getAdj (u, w) es
    | u == end = (end, start, weight):getAdj (u, w) es
    | otherwise = getAdj (u, w) es

relaxEdges :: [(Integer, Integer)] -> [(Integer, Integer, Integer)] -> [(Integer, Integer)]
relaxEdges distance [] = distance
relaxEdges ((start, weight):ds) ((start', end, weight'):as)
    | weight + weight' < prevAdjWeight = [y | y <- ] -- update distance
    where
        z = fromIntegral(end - 1)
        prevAdjWeight = distance !! z -- index distance with adj vertex z
        d = (start, weight):ds -- rename distance to use in list comprehension

            -- [(Node, Weight)], [Distance], [Adj])
dijkstra :: ([(Integer, Integer)], [Integer], [(Integer, Integer, Integer)]) -> 
            ([(Integer, Integer)], [Integer], [(Integer, Integer, Integer)])
dijkstra (q:qs,  d:ds, a:as)
    | length qs < 1 = ([q], d:ds, a:as)
    | otherwise = dijkstra ((queue qs), d:ds, a:as)
    where
        -- relax edges to adj nodes
        adj = getAdj q (a:as)
        (u, _) = q
        -- relaxedEdges = relaxEdges u (d:ds) adj

(n, it, adj) = problem1
distance = [0] ++ listFrom (nodes - 1) 999999
-- q contains the vertices in distance along with distance[i]
q = queue [ (y + 1, distance !! (fromIntegral y)) | y <- [0..nodes - 1] ]
x = dijkstra (q, distance, adj)

