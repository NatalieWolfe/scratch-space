#! /bin/bash

clang++ -std=c++1y -lssl -lcrypto $1 && ./a.out
#rm a.out
