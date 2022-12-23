#!/bin/sh

/usr/local/bin/g++-12 -std=gnu++17  -Wextra -O2 -DONLINE_JUDGE -D=LOCAL -Wl,-stack_size -Wl,0x11110000 toolkit/sample/sample_A.cpp

./toolkit/judge.sh toolkit/generator/testcase.txt toolkit/visualizer/default.json ./a.out

rm ./a.out
