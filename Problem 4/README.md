# Problem 4: Ant and seeds

### Problem Description

A laborious ant walks randomly on a 5x5 grid. The walk starts from the central square. At each step, the ant moves to an adjacent square at random (up, down, left, right), without leaving the grid; thus there are 2, 3 or 4 possible moves at each step depending on the ant's position.

At the start of the walk, a seed is placed on each square of the lower row. When the ant isn't carrying a seed and reaches a square of the lower row containing a seed, it will start to carry the seed. The ant will drop the seed on the first empty square of the upper row it eventually reaches.

### Questions

1) What's the expected number of steps until all seeds have been dropped in the top row? Give your answer rounded to 6 decimal places.

2) How many total runs did it take for your answer to converge? (Or you can just do a total of 10 million runs and output the expected number)

### Inputs and Outputs

1. The program needs to detect the number, N, of threads that can be run concurrently on the system being used and create N threads to answer the two questions above. 
2. Then generate an output file called ProblemOne.txt that contains the following:
   1. Number of threads created: # 
   2. Expected number of steps: #.###### 
   3. Total number of runs needed for solution convergence: #
