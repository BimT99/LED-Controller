#!/bin/sh
set -ev

mkdir -p build

g++ -c \
    -ISOEM/soem \
    -ISOEM/osal \
    -ISOEM/osal/linux \
    -ISOEM/oshw/linux \
    -o ./build/main.a ./src/main.cpp
  
g++ ./build/main.a \
    -LSOEM/build -lsoem \
    -o ./build/run

rm ./build/main.a

echo "compiled!"
echo