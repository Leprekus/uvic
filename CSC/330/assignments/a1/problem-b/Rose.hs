-- Code provided to students in UVic CSC 330 Summer 2025
-- 
-- Author: Mike Zastre
--
-- The copyright for this code is held by Mike Zastre. It is
-- not to be uploaded or otherwise accessed by any online service
-- or online-facilitated tool (such as Copilot or ChatGPT, although
-- this list is no exhaustive).


import System.IO
import System.Environment (getArgs)
import Data.List

-- ProblemTuple is made up of:
-- (1) number of places in the map
-- (2) weight of items from friends at each place
-- (3) place-to-place distances (i.e., weighted edges)
---
type ProblemTuple = (Integer, [Integer], [(Integer, Integer, Integer)] )


-- Top level function: Because Haskell constrains the mixture of
-- of functions having I/O with functions having no I/O, we need
-- to use an IO monad (the "IO ()" syntax).  There should be no
-- need to modify this code unless you _really_ know what you are
-- doing in Haskell.  This has been written for you in order that
-- you may completely ignore I/O issues.
--
-- The first parameter if `stamps` is the name of the input file;
-- the second is the name of the output file.
--
-- THERE SHOULD BE NO NEED TO MODIFY THE CODE UP TO THE POINT
-- WITH THE COMMENT "-- STUDENT WORK BEGINS HERE".
--

main :: IO ()
main = do
    -- TODO: delete
    putStrLn result
    args <- getArgs
    case args of
        (inFile : outFile : _) -> rose inFile outFile
        _ -> putStrLn "Usage: rose <input file> <output file>"


rose :: String -> String -> IO ()
rose inName outName = do
    inFile <- openFile inName ReadMode
    outFile <- openFile outName WriteMode
    lineData <- hGetContents inFile
    let rawLines = lines lineData
    let problem = parseData rawLines
    let answers = process problem
    outputResults outFile answers
    hClose inFile
    hClose outFile


parseData :: [String] -> ProblemTuple
parseData (a:b:c:cs) = (num, weights, edges)
    where
        num   = read a :: Integer
        weights = string2ints (words b)
        edges = strings2edges cs


strings2edges :: [String] -> [(Integer, Integer, Integer)]
strings2edges [] = []
strings2edges edge_data = map string2edge edge_data
    where
    string2edge s = tuplify3 (map (read :: String -> Integer) (words s))
    tuplify3 [a, b, c] = (a, b, c)


string2ints :: [String] -> [Integer]
string2ints [] = []
string2ints (a:as) = (read a :: Integer) : string2ints as


ints2string :: [Integer] -> String
ints2string [] = ""
ints2string (a:[]) = show a
ints2string (a:as) = show a ++ " " ++ ints2string as


outputResults :: Handle -> String -> IO ()
outputResults outFile s = do
    hPutStrLn outFile s



--
-- STUDENT WORK BEGINS HERE
--
-- YOU ARE FORBIDDEN TO USE EITHER "do" OR "let" IN YOUR
-- SOLUTION. THE APPEARANCE OF EITHER OF THESE KEYWORDS
-- IN YOUR CODE MAY RESULT IN A FAILING GRADE FOR THIS PART
-- OF THE ASSIGNMENT.
--

process :: ProblemTuple -> String
process _ = "Nuttin' honey!"

problem1 = (6, -- number of locations
            [1,1,2,3,1,0],
            [(1,2,2),(2,3,3),(3,6,4),(1,4,4),(4,3,2),(4,5,3),(5,6,2)])

-- utils start
castToInt :: Integer -> Int
castToInt n = fromIntegral (n :: Integer) :: Int

castToInteger = fromInteger . toInteger

range :: Integer -> Integer -> Integer -> [Integer]
range step start end = takeWhile (<end) $ iterate (+step) start
zipThree :: [a] -> [b] -> [c] -> [(a, b, c)]

zipThree [] [] [] = []
zipThree (x:xs) (y:ys) (z:zs) = (x, y, z) : zipThree xs ys zs
-- utils end
distance :: Integer -> Integer -> [Integer]
-- n - 1 accounts for v = 1 at 0th index having a starting distance of 0
distance n initial = [0] ++ take (castToInt n - 1) (repeat initial)
queue :: [(Integer, Integer, Integer)] -> [(Integer, Integer, Integer)]
queue [] = []
queue (x:xs) =
    queue [ y | y <- xs, y <= x ]
          ++ [x]
          ++ queue [ y | y <- xs, y > x]
-- convert to integer: fromIntegral n
-- integer to int: 
processQueue :: a -> [a] -> [a]
processQueue a [] = a []
processQueue (x:xs) = processQueue x xs
 
dijkstra :: ProblemTuple -> v -> String
dijkstra (n, _, edges) _ | length edges <  castToInt(n - 1) = "endpoint inaccessible"
dijkstra (n, items, edges) _ = show q
            where d = distance n 999999
                  q = queue (zipThree d (range 1 0 n) items) -- (ith-distance, ith-node, ith-item)
                  z = processQueue (head q) q
result = dijkstra problem1 1
