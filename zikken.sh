#!/usr/bin/env bash



for i in {1..200};do
    echo $i
    cd toolkit/generator
    j=$(($i+1))
    
    sed -e "s/seed = $i/seed = $j/" config.toml > tmp
    mv tmp config.toml
    cd ../../
    sh make.sh
    sh a.sh
done;


