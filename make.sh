#!/usr/bin/env bash

cd toolkit/generator


# 設定ファイル(config.toml)を読み込みテストケース(testcase.txt)生成
./random_world.py  -s 0 -c  config.toml> testcase.txt

cd ../../

