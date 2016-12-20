#include "../include/asm_io.h"

void io_outb(uint16_t a_port, uint8_t a_val)
{
    asm volatile("outb %0, %1" : : "a"(a_val), "Nd"(a_port));
}

void io_outw(uint16_t a_port, uint16_t a_val)
{
    asm volatile("outw %0, %1" : : "a"(a_val), "Nd"(a_port));
}

void io_outl(uint16_t a_port, uint32_t a_val)
{
    asm volatile("outl %0, %1" : : "a"(a_val), "Nd"(a_port));
}

uint8_t io_inb(uint16_t a_port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(a_port) );
    return ret;
}

uint16_t io_inw(uint16_t a_port)
{
    uint16_t ret;
    asm volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "Nd"(a_port) );
    return ret;
}

uint32_t io_inl(uint16_t a_port)
{
    uint32_t ret;
    asm volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "Nd"(a_port) );
    return ret;
}

void io_wait()
{
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}
