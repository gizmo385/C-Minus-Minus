#!/bin/bash

echo "Compiling..."
make clean compile 2>/dev/null >/dev/null

# Run the passing test code
echo "Running tests which should compile"
for test_file in $(ls 453tests/test*.c); do
    ./compile < $test_file 2>/dev/null >/dev/null
    if [ $? -ne 0 ]
    then
        echo $test_file ": Expected 0, got " $?
    fi
done

# Run the error tests
echo "Running tests which should NOT compile"
for test_file in $(ls 453tests/err*.c); do
    ./compile < $test_file 2>/dev/null >/dev/null
    if [ $? -ne 1 ]
    then
        echo $test_file ": Expected 1, got " $?
    fi
done
