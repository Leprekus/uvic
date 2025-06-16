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

distance = [0] ++ listFrom (nodes - 1) 999999
-- q contains the vertices in distance along with distance[i]
q = queue [ (y + 1, distance !! (fromIntegral y)) | y <- [0..nodes - 1] ]
getAdj :: (Integer, Integer) -> [(Integer, Integer, Integer)] -> [(Integer, Integer, Integer)]
getAdj _ [] = []
getAdj (u, w) edges = filter (\(u', z, w') -> (u == u') || (u == z)) edges

(n, it, adj) = problem1
            -- [(Node, Weight)], [Distance], [Adj])
dijkstra :: ([(Integer, Integer)], [Integer], [(Integer, Integer, Integer)]) -> 
            ([(Integer, Integer)], [Integer], [(Integer, Integer, Integer)])
dijkstra (q:qs,  d:ds, a:as)
    | length qs < 1 = ([q], d:ds, a:as)
    | otherwise = dijkstra ((queue qs), d:ds, a:as)
    where
        -- relax edges to adj nodes
        adj = getAdj q (a:as)


