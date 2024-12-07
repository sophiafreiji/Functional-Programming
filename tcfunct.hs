import Data.Time.Clock (getCurrentTime, diffUTCTime)
import Control.DeepSeq (NFData, deepseq)
import GHC.Generics (Generic)

data Stack a = EmptyStack | StackNode a (Stack a) deriving (Show, Generic)
instance NFData a => NFData (Stack a)

push :: a -> Stack a -> Stack a
push x s = StackNode x s


pop :: Stack a -> Stack a
pop EmptyStack = EmptyStack
pop (StackNode _ s) = s


estimateMemorySize :: Stack a -> Int -> Int
estimateMemorySize EmptyStack _ = 0
estimateMemorySize (StackNode _ s) elementSize = elementSize + estimateMemorySize s elementSize


measureTime :: String -> IO a -> IO a
measureTime description action = do
    start <- getCurrentTime
    result <- action
    end <- getCurrentTime
    putStrLn $ description ++ " took " ++ show (diffUTCTime end start)
    return result

forceEvaluation :: NFData a => a -> IO ()
forceEvaluation x = x `deepseq` return ()


testStackOperations :: Int -> IO ()
testStackOperations numElements = do
    let elementSize = 8 

    putStrLn $ "\nTesting " ++ show numElements ++ " elements:"

    
    measureTime "Push operation" $ do
        let stack = foldl (\s x -> push x s) EmptyStack [1..numElements]
        forceEvaluation stack
        putStrLn $ "Memory usage after push: " 
                 ++ show (estimateMemorySize stack elementSize) 
                 ++ " bytes"

    
    measureTime "Pop operation" $ do
        let stack = foldl (\s x -> push x s) EmptyStack [1..numElements]
        let finalStack = foldl (\s _ -> pop s) stack [1..numElements]
        forceEvaluation finalStack


main :: IO ()
main = do
    testStackOperations 10000
    testStackOperations 20000
    testStackOperations 30000
