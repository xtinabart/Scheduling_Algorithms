# CPU Scheduling Algorithms Simulator

## Author
Christina Bartsch  

## Date
February 16, 2025  

## Overview
This project simulates four different CPU scheduling algorithms: First Come First Serve (FCFS), Shortest Job First (SJF), Priority Scheduling, and Round Robin (RR). Based on command-line input, the program executes the specified algorithm and outputs the waiting time and turnaround time for each process.

## Building the Software
The included Makefile provides the following commands:

- `make` – Compiles the software  
- `make check` – Compiles and runs the provided test cases  
- `make clean` – Removes compiled files  

## Running the Software
The program is executed via command line with the following usage:

./scheduler -s <algorithm> [-q <quantum>] <input_file>

- `-s <algorithm>` specifies the scheduling algorithm:  
  - `1` - First Come First Serve  
  - `2` - Shortest Job First  
  - `3` - Priority Scheduling  
  - `4` - Round Robin (requires `-q` flag for time quantum)  

Example usage:

./scheduler -s 2 tests/test2.txt ./scheduler -s 4 -q 3 tests/test4.txt

## Testing
The software has been tested using both predefined test cases and additional stress tests to validate accuracy and efficiency.

| Test | Description | Test File |
|------|------------|-----------|
| **Test 1** | 5 processes, random order, basic functionality check | `tests/test1.txt` |
| **Test 2** | 100 processes, ordered process IDs, performance check | `tests/test2.txt` |
| **Test 3** | 300 processes, priority scheduling focus, priorities from 1-3 | `tests/test3.txt` |
| **Test 4** | 1000 processes, large input case, stability check | `tests/test4.txt` |
| **Test 5** | 5000 processes, stress test for scalability | `tests/test5.txt` |
