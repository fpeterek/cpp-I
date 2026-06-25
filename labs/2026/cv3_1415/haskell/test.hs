import Data.List (intercalate)

power :: Integral a => a -> a -> a
power base 0 = 1
power base exp = base * power base (pred exp)

squares :: Integral a => [a] -> [a]
squares = map (`power` 2)

evenOnly :: Integral a => [a] -> [a]
evenOnly = filter even

range :: Int -> Int -> [b] -> [b]
range begin end = (take items) . (drop begin)
    where
        items = end - begin

main :: IO ()
main = do
    let numbers = [1,2..]

    let list = range 15 30 $ squares $ evenOnly numbers

    let strings = map show list
    let joined = intercalate ", " strings
    let output = "[" ++ joined ++ "]"

    putStrLn output
