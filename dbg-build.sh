#!/bin/sh
rm -rf out
mkdir out
cd out
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
./tests
