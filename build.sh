#!/bin/sh

set -xe

CXX="${CXX:-c++}"
CXX_FLAGS="-Ietna-linux_amd64/include -Letna-linux_amd64/lib"
LIBS="-lm -letna -lglfw3 -lvulkan -lX11 -llua"
SRC="src/*.cpp"

$CXX $CXX_FLAGS -o bin/y3 $SRC $LIBS
