#ifndef _PhiOS_asm_io
#define _PhiOS_asm_io

#include "types.h"

void io_outb(
    uint16 a_port, 
    uint8 a_val
);

void io_outw(
    uint16 a_port, 
    uint16 a_val
);

void io_outl(
    uint16 a_port, 
    uint32 a_val
);

uint8 io_inb(
    uint16 a_port
);

uint16 io_inw(
    uint16 a_port
);

uint32 io_inl(
    uint16 a_port
);

void io_wait();

#endif
