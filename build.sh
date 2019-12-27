#!/bin/sh
rm -rf out
mkdir out
cd out
cmake ..
make
./tests
