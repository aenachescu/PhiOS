#include "kernel/include/arch/x86/asm_io.h"

void io_outb(uint16 a_port, uint8 a_val)
{
    asm volatile("outb %0, %1" : : "a"(a_val), "Nd"(a_port));
}

void io_outw(uint16 a_port, uint16 a_val)
{
    asm volatile("outw %0, %1" : : "a"(a_val), "Nd"(a_port));
}

void io_outl(uint16 a_port, uint32 a_val)
{
    asm volatile("outl %0, %1" : : "a"(a_val), "Nd"(a_port));
}

uint8 io_inb(uint16 a_port)
{
    uint8 ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(a_port) );
    return ret;
}

uint16 io_inw(uint16 a_port)
{
    uint16 ret;
    asm volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "Nd"(a_port) );
    return ret;
}

uint32 io_inl(uint16 a_port)
{
    uint32 ret;
    asm volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "Nd"(a_port) );
    return ret;
}

void io_wait()
{
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}
