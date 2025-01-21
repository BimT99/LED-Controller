#!/bin/sh

cd SOEM
mkdir -p build
cd build
cmake ..
make
cd ../../
mkdir -p build/lib
cp SOEM/build/libsoem.a build/lib
