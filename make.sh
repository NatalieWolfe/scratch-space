#! /bin/bash

file=$1

brewOpenSSL=`brew --prefix openssl`

CC="clang++"
CXXFLAGS="-std=c++1y -I$brewOpenSSL/include"
LDFLAGS="-L$brewOpenSSL/lib -lssl -lcrypto"

cmd="$CC $CXXFLAGS $LDFLAGS $file && ./a.out"

echo $cmd
eval $cmd

rm a.out
