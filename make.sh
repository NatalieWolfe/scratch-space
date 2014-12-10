#! /bin/bash

clang++ -std=c++11 $1 && ./a.out
rm a.out
