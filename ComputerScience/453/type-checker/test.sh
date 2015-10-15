#!/bin/bash

make clean compile

for test in $(ls 453tests/*.c); do
    ./compile < $test 2>/dev/null >/dev/null
    echo $test " --> " $?
done
