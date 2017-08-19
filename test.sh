#!/bin/bash

function create_dir {
    if ! [ -d "$1" ]; then
        mkdir $1
    fi
    cd $1
}

function build_tests_x86_64 {
    create_dir bin
    create_dir x86_64

    cmake ../.. -DARCH:STRING=x86_64
    rc=$?
    if [[ $rc != 0 ]]; then
        echo "${red}cmake failed${reset} - $rc"
        exit $rc
    fi

    make
    rc=$?
    if [[ $rc != 0 ]]; then
        echo "${red}make failed${reset} - $rc"
        exit $rc
    fi
}

function build_tests_x86_32 {
    create_dir bin
    create_dir x86_32

    cmake ../.. -DARCH:STRING=x86_32
    rc=$?
    if [[ $rc != 0 ]]; then
        echo "${red}cmake failed${reset} - $rc"
        exit $rc
    fi

    make
    rc=$?
    if [[ $rc != 0 ]]; then
        echo "${red}make failed${reset} - $rc"
        exit $rc
    fi   
}

function running_tests {
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
}

# define colors
red=`tput setaf 1`
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

if [ $# -ne 2 ]; then
    echo "${red}Illegal number of parameters${reset}"
    exit 1
fi

echo -e "\n${green}*****${reset} Building tests"

if [ "$1" == "run" ];then

    if [ "$2" == "x86_64" ]; then
        build_tests_x86_64
        echo "${green}build tests for x86_64 successfully${reset}"
        running_tests
    fi

    if [ "$2" == "x86_32" ]; then
        build_tests_x86_32
        echo "${green}build tests for x86_32 successfully${reset}"
        running_tests
    fi

    echo "${red}invalid parameter for run command${reset}"
    exit 1
fi

echo "${red}invalid syntax${reset}"
exit 1
