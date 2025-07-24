
-- ghci
-- :l /path/to/filename
-- :l again unloads modules
-- ':' LHS is elements concatenated with RHS (list)  -> a:[a] is right associative
weightedSum [] [] = 0
weightedSum (x:xs) (y:ys) = x * y + weightedSum xs ys -- multiply the heads and call the function on the rest of the list


-- functions as values:
-- '.' produces a function
-- ex: (f . g)x = f (g x)
-- anonymous functions \m, n -> m + n
-- map signature: map :: (a -> b) -> [a] -> [b] // accepts a funtion and list [a] and returns list [b] with function applied
--m :: (a -> b) -> [a] -> [b]
m f (x:xs) = f . x ++ m f xs

