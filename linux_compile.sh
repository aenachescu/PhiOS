#!/bin/bash

# if parameter $1 doesn't exists then the script will compile the OS and it will
# generate the iso file for ia64.

# if $1 is "build" then the script will compile the OS and it will generate the
# iso file for ia64.

# if $1 is "build" and $2 is not equal with "" then the script will compile the
# OS and it will generate the iso file for the architecture from $2 parameter.

# if $1 is "clean-all" then the script will delete bin directory.

# if $1 is "clean" and $2 is not equal with "" then the script will delete the
# bin/architecture_name directory.

# $2 can take the following values: ia32, ia64

function create_bin {
    if ! [ -d "bin" ];
    then
        mkdir "bin"
    fi
    cd bin
}

function create_ia32 {
    if ! [ -d "ia32" ];
    then
        mkdir ia32
    fi
    cd ia32
}

function create_ia64 {
    if ! [ -d "ia64" ];
    then
        mkdir ia64
    fi
    cd ia64
}

function remove_dir {
    if [ -d "$1" ];
    then
        rm -rf $1
    fi
}

export PATH=$PWD/build/linux:$PATH

if [ "$1" == "" ];
then
    create_bin
    create_ia64
    cmake ../.. -DARCH:STRING=x86_64
    make
    make iso
fi

if [ "$1" == "build" ];
then
    if [ "$2" == "ia32" ];
    then
        create_bin
        create_ia64
        cmake ../.. -DARCH:STRING=x86_64
        make
        make iso
    fi

    if [ "$2" == "ia64" ];
    then
        create_bin
        create_ia32
        cmake ../.. -DARCH:STRING=x86_32
        make
        make iso
    fi
fi

if [ "$1" == "clean-all" ];
then
    remove_dir bin
fi

if [ "$1" == "clean" ];
then
    if [ "$2" == "ia32" ];
    then
        remove_dir bin/ia32
    fi

    if [ "$2" == "ia64" ];
    then
        remove_dir bin/ia64
    fi
fi
