-- name :: p1 -> p2 -> ... pk -> pk+1
-- name is the function's identifier
-- p1 ... pk are the parameters the function takes
-- pk+1 is the return type of the fn 
main :: IO() 
main = do
  let unsorted = [1,2,3]
  putStrLn $ show $ unsorted
