#!/bin/bash

rm avl.c
gcc -E avl_impl.c -o avl_tmp.c -I../../../ -P
clang-format avl_tmp.c >> avl.c
rm avl_tmp.c
