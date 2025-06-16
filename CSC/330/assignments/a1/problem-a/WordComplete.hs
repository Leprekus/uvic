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
import Debug.Trace

type ProblemTuple = (String, String, [String])

main :: IO ()
main = do
    args <- getArgs
    case args of
        (inFile : outFile : _) -> wordComplete inFile outFile
        _ -> putStrLn "Usage: <input file> <output file>"

wordComplete:: String -> String -> IO ()
wordComplete inName outName = do
    inFile <- openFile inName ReadMode
    outFile <- openFile outName WriteMode
    lineData <- hGetContents inFile
    let rawLines = lines lineData
    case rawLines of
        [] -> putStrLn "Input file is empty."
        (numTests : tests) -> do
            let problemList = parseData tests -- i) inxx.txt file contents
            let answers = process problemList -- ii ) 'i)' fed into process
            outputResults outFile answers
    hClose inFile
    hClose outFile

parseData :: [String] -> [ProblemTuple]
parseData [] = []
parseData (want:have:opt1:opt2:opt3:theRest) =
    (want, have, [opt1, opt2, opt3]) : parseData theRest
parseData _ = []


outputResults :: Handle -> [String] -> IO ()
outputResults _ [] = return ()
outputResults outfile (a:as) = do
    hPutStrLn outfile a
    outputResults outfile as

--
-- STUDENT WORK BEGINS HERE
--
-- YOU ARE FORBIDDEN TO USE EITHER "do" OR "let" IN YOUR
-- SOLUTION. THE APPEARANCE OF EITHER OF THESE KEYWORDS
-- IN YOUR CODE MAY RESULT IN A FAILING GRADE FOR THIS PART
-- OF THE ASSIGNMENT.
--

process :: [ProblemTuple] -> [String]
process t = map (\a -> show $ processTuple a) t
-- process _ = ["nuttin' honey!"] -- final answer goes here
a = process problem1 
problem1 = [ ("COOKIEMONSTER", "COO", 
                ["COOKIEEATER", "COURAGEOUS", "COOKIEMONSTUR"]),
             ("FOUNTAINPEN", "MORDOR",
                ["BEER", "FEEBLE", "ANDROID"]) ]


{- 
 -  case 1: taps = (needleLen - charMatch) + (strLen - charMatch)
 -      needle: a a a a
 -      str   : a a
 - case 2: taps = (strLen - charMatch) + (neeedleLen - charMatch)
 -      needle: a a
 -      str   : a a a a
 - so case 1 == case 2 === case 3
 - case 3: taps = (strLen - charMatch)
 -      needle: a a a a
 -      str   : a a a a
 -}
 -- count a contiguous sequence of matching characters in two strings and return its length
countContigousChars :: String -> String -> Int
countContigousChars str1 str2 = length $ takeWhile (\a -> fst a == snd a) (zip str1 str2)
-- x = countContigousChars "aaab" "aa"
(needle, str, strArr) = head problem1
-- count the taps required for a given string to be equal to "needle"
countTaps :: String -> String -> Int
ountTaps "" "" = 0
countTaps needle str = (needleLen - charMatch) + (strLen - charMatch)
                      where 
                          needleLen = length needle
                          strLen = length str
                          charMatch = countContigousChars needle str

-- return the minimum number of taps required for a string in an array of strings to become needle
processStrArr :: String -> [String] -> Int
processStrArr needle strArr = (minimum . map (\a -> countTaps needle a + 1)) strArr -- include the cost of selecting the option
-- y =  processStrArr "" strArr
processTuple :: ProblemTuple -> Int
processTuple (needle, str, strArr) = min (countTaps needle str) (processStrArr needle strArr) 
-- z = map (\a -> processTuple a) problem1

