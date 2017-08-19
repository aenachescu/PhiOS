#!/bin/bash

green=`tput setaf 2`
blue=`tput setaf 4`
reset=`tput sgr0`

# building CUT library
echo -e "${green}*****${reset} Building CUT"
cd CUT/src
make clean
make
cd ../..

# set path to cmake
export PATH=$PWD/build/linux-x86_64/cmake/bin:$PATH

# Change to tests directory
cd tests

# building tests
echo -e "\n${green}*****${reset} Building tests"
cmake .
make clean
make

err=$?
if [[ $err != 0 ]]; then
    echo "make failed - $err"
    exit $err
fi

# running all tests
echo -e "\n${green}*****${reset} Running tests"
rc=0

# running PAA tests
echo -e "${blue}***${reset} Running test_PAA"
./test_paa
rc=$((rc + $?))

# running BitmapPMA tests
echo -e "\n${blue}***${reset} Running test_BitmapPMA"
./test_bitmap
rc=$((rc + $?))

exit $rc
