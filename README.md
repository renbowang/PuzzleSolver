This program solves a unique Russian puzzle problem. Given a puzzle board (the target shape) and a set of puzzle pieces, it determines if the pieces can perfectly fill the board without overlap or gaps—using all pieces exactly once. Each piece can be rotated but cannot be flipped.

If a solution exists, the program outputs one valid arrangement of the pieces fitting the puzzle.

Compile
g++ -std=c++17 -O2 -o puzzle_solver.exe puzzle_solver.cpp

Run
./puzzle_solver.exe S1-input.txt
