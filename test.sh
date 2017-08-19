#!/bin/bash

# Compile and install CUT
cd CUT/src
make clean
make
cd ..
sudo ./install.sh
cd ..

# Change to tests directory
cd tests

# Configure and compile
cmake .
make clean
make

# Run and return
rc=0
./test_paa
rc=$((rc + $?))
./test_bitmap
rc=$((rc + $?))
exit $rc