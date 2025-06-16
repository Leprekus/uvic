addPairs :: [(Integer,Integer)] -> [Integer]
addPairs pairList = [ m+n | (m, n) <- pairList]

minOfList :: [Integer] -> Maybe Integer
minOfList [] = Nothing
minOfList list = min (head list) minOfList
