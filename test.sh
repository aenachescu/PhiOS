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
make

# Run and return
./tests
rc=$?
exit $rc