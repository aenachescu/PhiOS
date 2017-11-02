#!/bin/bash

# define colors
red=`tput setaf 1`
green=`tput setaf 2`
blue=`tput setaf 4`
reset=`tput sgr0`

function create_dir {
    if ! [ -d "$1" ]; then
        mkdir $1
    fi
    cd $1
}

function remove_dir {
    if [ -d "$1" ]; then
        rm -rf $1
    fi
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
    test_paa_rc = 0
    test_bitmapPMA_rc = 0
    test_kstdlib_rc = 0
    test_logging_rc = 0
    test_avl_rc = 0
    rc = 0

    # running PAA tests
    echo -e "${blue}***${reset} Running test_PAA"
    ./test_paa
    test_paa_rc=$?

    # running BitmapPMA tests
    echo -e "\n${blue}***${reset} Running test_BitmapPMA"
    ./test_bitmap
    test_bitmapPMA_rc=$?

    # running kstdlib tests
    echo -e "\n${blue}***${reset} Running test_kstdlib"
    ./test_kstdlib
    test_kstdlib_rc=$?

    # running logging tests
    echo -e "\n${blue}***${reset} Running test_logging"
    ./test_logging
    test_logging_rc=$?

    # running avl tests
    echo -e "\n${blue}***${reset} Running test_avl"
    ./test_avl
    test_avl_rc=$?

    if [[ $test_paa_rc != 0 ]]; then
        echo -e "${red}test_paa failed${reset}"
        rc=1
    fi

    if [[ $test_bitmapPMA_rc != 0 ]]; then
        echo -e "${red}test_bitmapPMA failed${reset}"
        rc=1
    fi

    if [[ $test_kstdlib_rc != 0 ]]; then
        echo -e "${red}test_kstdlib failed${reset}"
        rc=1
    fi

    if [[ $test_logging_rc != 0 ]]; then
        echo -e "${red}test_logging failed${reset}"
        rc=1
    fi

    if [[ $test_avl_rc != 0 ]]; then
        echo -e "${red}test_avl failed${reset}"
        rc=1
    fi

    exit $rc
}

function configEnvironment {
    # building CUT library
    echo -e "${green}*****${reset} Building CUT"
    cd CUT/src
    make clean
    make $1
    cd ../..

    # set path to cmake
    export PATH=$PWD/build/linux-x86_64/cmake/bin:$PATH

    # Change to tests directory
    cd tests
}

if [ $# -ne 2 ]; then
    echo "${red}Illegal number of parameters${reset}"
    exit 1
fi

if [ "$1" == "run" ];then
    if [ "$2" == "x86_64" ]; then
        configEnvironment platform=-m64

        echo -e "\n${green}*****${reset} Building tests for x86_64"
        build_tests_x86_64
        echo "${green}builded tests for x86_64 successfully${reset}"

        running_tests
    fi

    if [ "$2" == "x86_32" ]; then
        configEnvironment platform=-m32

        echo -e "\n${green}*****${reset} Building tests for x86_32"
        build_tests_x86_32
        echo "${green}builded tests for x86_32 successfully${reset}"

        running_tests
    fi

    echo "${red}invalid parameter for run command${reset}"
    exit 1
fi

if [ "$1" == "clean" ];then
    cd tests

    if [ "$2" == "x86_32" ]; then
        remove_dir bin/x86_32
        echo "${green}clean x86_32 successfully${reset}"
        exit 0
    fi

    if [ "$2" == "x86_64" ]; then
        remove_dir bin/x86_64
        echo "${green}clean x86_64 successfully${reset}"
        exit 0
    fi

    echo "${red}invalid parameter for clean command${reset}"
    exit 1
fi

echo "${red}invalid syntax${reset}"
exit 1
