# Stack-Puzzle

_A unique-solution Russian puzzle solver._

---

## About

This program solves a Russian puzzle where a given set of pieces must perfectly fill a target board without overlap or gaps. Each piece can be rotated but **cannot be flipped**.

If a solution exists, it outputs one valid arrangement.

---

## How to Build and Run

### Compile

```bash
g++ -std=c++17 -O2 -o puzzle_solver.exe puzzle_solver.cpp
```

### Run

```bash
./puzzle_solver.exe S1-input.txt
```

---

## Example

### Input
```bash
1 1 0 1 1
1 0 1 0 1
1 1 1 1 1
1 1 1 1 1
0 1 0 1 1
x
0 1 0
1 1 1
0 1 0
x
2 0
2 2
x
0 0 0
0 3 3
3 3 0
x
4 4
4 4
x
0 0 0
0 0 5
5 5 5
x
0
```

### Output
```bash
2 2 0 5 5
2 0 1 0 5
3 1 1 1 5
3 3 1 4 4
0 3 0 4 4
```
