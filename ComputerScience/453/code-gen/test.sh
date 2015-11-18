#!/bin/bash

directory=SPIM_codegen_milestone_1_tests

echo "Compiling..."
make clean compile 2>/dev/null >/dev/null

# Run the passing test code
echo "Running tests..."
for test_num in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27; do
    echo Running test${test_num}.c
    ./compile < ${directory}/test${test_num}.c > test${test_num}.s
    spim -file test${test_num}.s | tail -n +2 > test${test_num}.out
    diff test${test_num}.out SPIM_codegen_milestone_1_tests/out${test_num}
    rm *.s *.out
done
