#!/bin/bash

# if parameter $1 doesn't exists then the script will compile the OS and it will
# generate the iso file for x86_64.

# if $1 is "build" then the script will compile the OS and it will generate the
# iso file for x86_64.

# if $1 is "build" and $2 is not equal with "" then the script will compile the
# OS and it will generate the iso file for the architecture from $2 parameter.

# if $1 is "clean-all" then the script will delete bin directory.

# if $1 is "clean" and $2 is not equal with "" then the script will delete the
# bin/architecture_name directory.

# $2 can take the following values: x86_32, x86_64





# helper functions

function create_bin {
    if ! [ -d "bin" ];
    then
        mkdir "bin"
    fi
    cd bin
}

function create_x86_32 {
    if ! [ -d "x86_32" ];
    then
        mkdir x86_32
    fi
    cd x86_32
}

function create_x86_64 {
    if ! [ -d "x86_64" ];
    then
        mkdir x86_64
    fi
    cd x86_64
}

function remove_dir {
    if [ -d "$1" ];
    then
        rm -rf $1
    fi
}

function build_x86_32 {
    create_bin
    create_x86_32

    cmake ../.. -DARCH:STRING=x86_32
    rc=$?; if [[ $rc != 0 ]]; then echo "cmake failed - $rc"; exit $rc; fi

    make
    rc=$?; if [[ $rc != 0 ]]; then echo "make failed - $rc"; exit $rc; fi
    
    make iso
    rc=$?; if [[ $rc != 0 ]]; then echo "make iso failed - $rc"; exit $rc; fi
    
}

function build_x86_64 {
    create_bin
    create_x86_64

    cmake ../.. -DARCH:STRING=x86_64
    rc=$?; if [[ $rc != 0 ]]; then echo "cmake failed - $rc"; exit $rc; fi

    make
    rc=$?; if [[ $rc != 0 ]]; then echo "make failed - $rc"; exit $rc; fi

    make iso
    rc=$?; if [[ $rc != 0 ]]; then echo "make iso failed - $rc"; exit $rc; fi    
}





# set environment

if [ "$1" == "" ];
then
    export PATH=$PWD/build/linux-x86_64/gcc-x86_64/bin:$PWD/build/linux-x86_64/cmake/bin:$PATH
fi

if [ "$1" == "build" ];
then
    if [ "$2" == "x86_32" ];
    then
        export PATH=$PWD/build/linux-x86_64/gcc-x86_32/bin:$PWD/build/linux-x86_64/cmake/bin:$PATH
    fi

    if [ "$2" == "x86_64" ];
    then
        export PATH=$PWD/build/linux-x86_64/gcc-x86_64/bin:$PWD/build/linux-x86_64/cmake/bin:$PATH
    fi
fi

if [ "$1" == "build-all" ];
then
    export PATH=$PWD/build/linux-x86_64/gcc-x86_32/bin:$PWD/build/linux-x86_64/cmake/bin:$PATH
    export PATH=$PWD/build/linux-x86_64/gcc-x86_64/bin:$PWD/build/linux-x86_64/cmake/bin:$PATH
fi





# build

if [ "$1" == "" ];
then
    build_x86_64
fi

if [ "$1" == "build" ];
then
    if [ "$2" == "x86_64" ];
    then
        build_x86_64
    fi

    if [ "$2" == "x86_32" ];
    then
        build_x86_32
    fi
fi

if [ "$1" == "build-all" ];
then
    build_x86_32
    cd ../..
    build_x86_64
fi





# clean-up

if [ "$1" == "clean-all" ];
then
    remove_dir bin
fi

if [ "$1" == "clean" ];
then
    if [ "$2" == "x86_32" ];
    then
        remove_dir bin/x86_32
    fi

    if [ "$2" == "x86_64" ];
    then
        remove_dir bin/x86_64
    fi
fi
