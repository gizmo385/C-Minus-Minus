#!/bin/bash

directory=tests/code_gen
num_tests=$(ls -l ${directory} | grep -E "test\d+\.c" | wc -l)

echo "Compiling compiler..."
make clean debug 2>/dev/null >/dev/null

# Run the passing test code
echo "Running tests..."
for test_num in `seq 1 ${num_tests}`; do
    test_file="${directory}/test${test_num}.c"
    correct_out="${directory}/out${test_num}"
    asm_file="${directory}/test${test_num}.s"
    output_file="${directory}/test${test_num}.out"

    echo Compiling and running ${test_file} with native C compiler...
    gcc -w -o a.out ${test_file} ${directory}/print.c
    ./a.out > ${correct_out}
    rm a.out

    echo Compiling ${test_file} with compiler:
    ./compile < ${test_file} > ${asm_file}
    gtimeout 3 spim -file ${asm_file} | tail -n +2 > ${output_file}
    diff --suppress-common-lines --side-by-side ${output_file} ${correct_out}
    mv ./debug.log tests/debug_logs/debug${test_num}.log 2>/dev/null || true
done
