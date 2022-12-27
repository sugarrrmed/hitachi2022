#!/bin/sh

#/usr/local/bin/g++-12 -std=gnu++17  -I /usr/local/include -Wextra -O2 -DONLINE_JUDGE -D=LOCAL -Wl,-stack_size -Wl,0x11110000 hitachi/main.cpp

/usr/local/bin/g++-12 -std=gnu++17                         -Wextra -O2 -DONLINE_JUDGE -D=LOCAL -Wl,-stack_size -Wl,0x11110000 hitachi/pre.cpp

./toolkit/judge.sh toolkit/generator/testcase.txt toolkit/visualizer/default.json ./a.out

rm ./a.out
