#!/bin/bash

#directory=453tests
#directory=type_checking_tests
directory=debray_type_checking_tests

echo "Compiling..."
make clean compile 2>/dev/null >/dev/null

# Run the passing test code
echo "Running tests which should compile"
for test_file in $(ls ${directory}/test*.c); do
    ./compile < $test_file 2>/dev/null >/dev/null
    res=$?
    if [ $res -ne 0 ]
    then
        echo $test_file ": Expected 0, got " $res
    fi
done

# Run the error tests
echo "Running tests which should NOT compile"
for test_file in $(ls ${directory}/err*.c); do
    ./compile < $test_file 2>/dev/null >/dev/null
    res=$?
    if [ $res -ne 1 ]
    then
        echo $test_file ": Expected 1, got " $res
    fi
done
