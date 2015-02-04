#! /bin/bash

file=$1

CC="clang++"
CXXFLAGS="-std=c++1y"
LDFLAGS="-L`brew --prefix openssl`/lib -lssl -lcrypto"

cmd="$CC $CXXFLAGS $LDFLAGS $file && ./a.out"

echo $cmd
eval $cmd

rm a.out
