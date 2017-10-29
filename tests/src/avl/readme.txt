How to generate avl.c

rm avl.c
gcc -E avl_impl.c -o avl_tmp.c -I../../../ -P
clang-format avl_tmp.c >> avl.c
rm avl_tmp.c

In avl.c is implemented AVL tree and that file is used in
unit testing. Every time the implementation changes, this
file needs to be regenerated. Is a compromise to be able
to see test coverage.
