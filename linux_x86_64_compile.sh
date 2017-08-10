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


function remove_dir {
    if [ -d "$1" ]; then
        rm -rf $1
    fi
}

function create_dir {
    if ! [ -d "$1" ]; then
        mkdir $1
    fi
    cd $1
}

function build_x86_32 {
    export PATH=$PWD/build/linux-x86_64/gcc-x86_32/bin:$PWD/build/linux-x86_64/cmake/bin:$PATH
    
    create_dir bin
    create_dir x86_32

    cmake ../.. -DARCH:STRING=x86_32
    rc=$?
    if [[ $rc != 0 ]]; then
        echo "cmake failed - $rc"
        exit $rc
    fi

    make
    rc=$?
    if [[ $rc != 0 ]]; then
        echo "make failed - $rc"
        exit $rc
    fi

    if [ "$1" == "false" ]; then
        make iso

        rc=$?
        if [[ $rc != 0 ]]; then
            echo "make iso failed - $rc"
            exit $rc
        fi
    fi    
}

function build_x86_64 {
    export PATH=$PWD/build/linux-x86_64/gcc-x86_64/bin:$PWD/build/linux-x86_64/cmake/bin:$PATH
    
    create_dir bin
    create_dir x86_64

    cmake ../.. -DARCH:STRING=x86_64
    rc=$?
    if [[ $rc != 0 ]]; then
        echo "cmake failed - $rc"
        exit $rc
    fi

    make
    rc=$?
    if [[ $rc != 0 ]]; then
        echo "make failed - $rc"
        exit $rc
    fi

    if [ "$1" == "false" ]; then
        make iso

        rc=$?
        if [[ $rc != 0 ]]; then
            echo "make iso failed - $rc"
            exit $rc
        fi
    fi
}

if [ $# -lt 2 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

if [ "$1" == "build" ];then

    if [ $# -gt 3 ]; then
        echo "invalid syntax for build command"
        exit 1
    fi

    notGenerateIso="false"
    if [ $# -eq 3 ]; then
        if [ "$3" == "notGenerateIso" ]; then
            notGenerateIso="true"
        else
            echo "invalid second parameter for build command"
            exit 1
        fi
    fi

    if [ "$2" == "x86_64" ]; then
        build_x86_64 $notGenerateIso
        echo "build x86_64 successfully"
        exit 0
    fi

    if [ "$2" == "x86_32" ]; then
        build_x86_32 $notGenerateIso
        echo "build x86_32 successfully"
        exit 0
    fi

    echo "invalid parameter for build command"
    exit 1
fi

if [ "$1" == "clean" ]; then

    if [ $# -ne 2 ]; then
        echo "invalid syntax for clean command"
        exit 1
    fi

    if [ "$2" == "x86_32" ]; then
        remove_dir bin/x86_32
        echo "clean x86_32 successfully"
        exit 0
    fi

    if [ "$2" == "x86_64" ]; then
        remove_dir bin/x86_64
        echo "clean x86_64 successfully"
        exit 0
    fi

    echo "invalid parameter for clean command"
    exit 1
fi

echo "invalid syntax"

exit 1
