# PhiOS ![Build Passing Icon](https://travis-ci.org/EnachescuAlin/PhiOS.svg?branch=devel)
[![Coverage Status](https://coveralls.io/repos/github/EnachescuAlin/PhiOS/badge.svg?branch=devel)](https://coveralls.io/github/EnachescuAlin/PhiOS?branch=devel)

## How to build?
* First of all, you must have installed a Linux based operating system.
* Run `linux_x86_64_getDependencies.sh` to obtain dependencies. (this will take a while)
* Run `linux_x86_64_compile.sh build x86_32` for the 32-bit ISO or `linux_x86_64_compile.sh build x86_64` for the 64-bit one. 

## How to run?
* The binaries of PhiOS are located in `bin/<arch_name>` directory.
* We recommend using QEMU for testing, the command you should use is `qemu-system-x86_64 -cdrom bin/<arch_name>/phios.iso -device isa-debug-exit,iobase=0xf4,iosize=0x04`.
* Any other virtual machine should work too, or even on real hardware.

## Dependencies
* gcc
* cmake
* xorriso
* grub2
* clang-format
* gcc-multilib
* libc6-dev-i386
