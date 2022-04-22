# Matrix Determinant Calculator

## Description
This repository is a concurrent program that uses shared memory and three processes (P1, P2, P3) to calculate the determinant of a 3x3 matrix of integers.

The mtrix M is denoted as:
```
M = a b c  
    d e f  
    g h i
```

When the program begins, each process will perform a compuation and, at the same time, determine the largest integer in the matrix. The computations are detoted as:
- P1 = a(ei-fh)
- P2 = b(fg-di)
- P3 = c(dh-eg)

When all the calculations are finished, all the processes are summed together to find the determinant of Matrix M. Overall, the program will print the following,
- the current process that is running,
- the determinant of the matrix,
- the largest integer in the matrix
- and the time it took for the program to complete

## Running the program
- To run the program, it must be set in a Linux environment
- Requires gcc version of 9.3.1 or higher

To compile the program, simply use the makefile by typing:
```
$ make
```
A new executable will be created, and simply run it by typing:
```
$ ./DET
```

## Instructions
To change the input of the matrix, simply locate the matrix under the file ```DET.c``` line 44, and change the numeric values. Below is a clearer representation of how the matrix is formatted.

If you wanted this matrix: 
```
M = 20 20 50 
    10  6 70  
    40  3  2  
```
then change line 44 to ```int M[3][3] = {{20, 20, 50}, {10, 6, 70}, {40, 3, 2}};```

### Example of output
Matrix:
```
M = 20 20 50 
    10  6 70  
    40  3  2  
```
Output:
```
Child Process: Working with element 1 of D
Child Process: Working with element 2 of D
Child Process: Working with element 0 of D
Value of R is 41140
Largest int is 70
Elapsed Time: 1791 micro seconds
```

## Credits
Author: Anthony Massaad

Copyright © 2021 Anthony Massaad. All rights reserved
