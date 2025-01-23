#!/bin/sh

cd SOEM
mkdir -p build
cd build
cmake ..
make
cd ../../