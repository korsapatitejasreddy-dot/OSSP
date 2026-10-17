
# Virtual Memory Page Replacement Visualizer

An OSSP hackathon project developed in C on Ubuntu Linux.

## Algorithms
- FIFO (First-In, First-Out)
- LRU (Least Recently Used)
- Optimal Page Replacement

## Features
- Step-by-step memory frame visualization
- Page hit and page fault counting
- Hit ratio and fault ratio calculation
- Comparison of all three algorithms

## Requirements
- Ubuntu Linux
- GCC compiler

## Compile
```bash
gcc -Wall -Wextra -std=c11 page_replacement.c -o visualizer
```

## Run
```bash
./visualizer
```
