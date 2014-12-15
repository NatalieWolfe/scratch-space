#! /bin/bash

clang++ -std=c++1y $1 && ./a.out
rm a.out
