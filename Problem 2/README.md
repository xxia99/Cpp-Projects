# Problem 2: Langton’s Ant

### Problem Description

An ant moves on a regular grid of squares that are colored either black or white. The ant is always oriented in one of the cardinal directions (left, right, up or down) and moves from square to adjacent square according to the following rules:

1. if it is on a black square, it flips the color of the square to white, rotates 90 degrees counterclockwise and moves forward one square.
2.  if it is on a white square, it flips the color of the square to black, rotates 90 degrees clockwise and moves forward one square.

Starting with a grid that is entirely white, you will need to determine how many squares are black after some number of moves of the ant?

### Inputs and Outputs

1. The program takes one command-line argument variable of type unsigned long that indicates the number of moves the ant makes.
2. The program outputs just the number of black squares to a text file called output2.txt.
