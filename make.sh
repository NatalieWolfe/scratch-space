#! /bin/bash

file=$1
shift

brewOpenSSL=`brew --prefix openssl`
brewCurl=`brew --prefix curl`

CC="$(brew --prefix gcc)/bin/g++-5"
CXXFLAGS="-g -std=c++1y -I$brewOpenSSL/include -I$brewCurl/include"
LDFLAGS="-L$brewOpenSSL/lib -lssl -lcrypto -L$brewCurl/lib -lcurl"

cmd="$CC $CXXFLAGS $LDFLAGS $file && ./a.out"

echo $cmd $@
eval $cmd $@

rm a.out
