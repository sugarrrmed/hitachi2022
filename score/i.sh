#!/bin/sh

echo "">error.txt

for seed in {1..10};do

    cd toolkit/generator

        ./random_world.py -g config.toml

        gsed '27d' config.toml > tmp
        gsed '27i type = "A" ' tmp>config.toml

        gsed '34d' config.toml > tmp
        gsed "34i seed = $seed" tmp > config.toml
        #mv tmp config.toml
        rm tmp
        
        ./random_world.py -c config.toml > testcase.txt

    cd ../../


    /usr/local/bin/g++-12 -std=gnu++17  -Wextra -O2 -DONLINE_JUDGE -D=LOCAL -Wl,-stack_size -Wl,0x11110000         beam.cpp

    ./toolkit/judge.sh toolkit/generator/testcase.txt toolkit/visualizer/default.json ./a.out 2>> error.txt

    rm ./a.out

done;

python logcalc.py
