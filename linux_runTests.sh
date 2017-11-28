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

    # update avl implementation
    cd ../../src/avl
    ./updateAvl.sh
    cd ../../bin/x86_64

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

    # update avl implementation
    cd ../../src/avl
    ./updateAvl.sh
    cd ../../bin/x86_32

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

function running_test {
    test_rc=0

    echo -e "${blue}***${reset} Running $1"
    ./$1
    test_rc=$?

    if [ $test_rc != 0 ]; then
        echo -e "${red}$1 failed${reset}"
    fi

    exit $test_rc
}

function running_tests {
    testsArr=(test_paa test_bitmap test_kstdlib test_logging test_avl test_x86_atomic test_x86_spinlock)
    failedTestsArr=()
    err=0

    echo -e "\n${green}*****${reset} Running tests"

    for item in ${testsArr[*]}
    do
        echo -e "\n${blue}***${reset} Running $item"
        ./$item
        if [ $? != 0 ]; then
            err=1
            failedTestsArr+=($item)
        fi
    done

    for item in ${failedTestsArr[*]}
    do
        echo -e "${red}$item failed${reset}"
    done

    exit $err
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

if [ $# -gt 3 ]; then
    echo "${red}Illegal number of parameters${reset}"
    exit 1
fi

if [ "$1" == "run" ];then
    if [ "$2" == "x86_64" ]; then
        configEnvironment platform=-m64

        echo -e "\n${green}*****${reset} Building tests for x86_64"
        build_tests_x86_64
        echo "${green}built tests for x86_64 successfully${reset}"
    elif [ "$2" == "x86_32" ]; then
        configEnvironment platform=-m32

        echo -e "\n${green}*****${reset} Building tests for x86_32"
        build_tests_x86_32
        echo "${green}built tests for x86_32 successfully${reset}"
    else
        echo "${red}invalid parameter for run command${reset}"
        exit 1
    fi

    if [ $# == 3 ]; then
        running_test $3
    fi

    running_tests
fi

if [ "$1" == "clean" ];then
    if [ $# -ne 2]; then
        echo "${red}invalid number of parameters for clean command${reset}"
        exit 1
    fi

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
