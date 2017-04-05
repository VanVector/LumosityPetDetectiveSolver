# LumosityPetDetectiveSolver
Lumosity Pet Detective Solver

## Input format:

1. Draw a map of current level.

Example:

    o-o-o-o
    |.|...|
    o.o-o-o
    |.|.|.|
    o-o-o.o
    |.....|
    o-o.o-o
    |...|.|
    o-o-o-o

2. Next put **nAnimals** and **maxSteps**
3. Then put 0-based starting detective coordinates **carPos.x** and **carPos.y**
4. In the next  **nAnimals** lines put 4 numbers: **(x, y)** coordinates of **i**-th pet and **(x, y)** coordinates of its home

## Output format:
You will receive a number of steps to take from last to first in order to complete level.

## Sample input (can be found in **in.txt** file):
	o-o-o-o
	|#|###|
	o#o-o-o
	|#|#|#|
	o-o-o#o
	|#####|
	o-o#o-o
	|###|#|
	o-o-o-o

	9
	25 
	2 2
	1 2 4 0
	1 3 2 1
	2 3 1 0
	4 3 0 1
	4 1 3 1
	3 0 0 2
	2 0 0 3
	1 1 0 0
	3 3 3 2 
  
## Sample output:
	0 3 drop
	0 2 drop
	0 1 drop
	0 0 drop
	0 1
	1 1 pick
	2 1 drop
	2 0 pick
	1 0 drop
	2 0
	3 0 pick
	3 1 drop
	3 0
	4 0
	4 1 pick
	4 0 drop
	4 1
	4 2
	4 3 pick
	3 3
	3 2 drop
	3 3 pick
	2 3 pick
	1 3 pick
	1 2 pick
	2 2
