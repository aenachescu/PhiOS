#ifndef _PhiOS_asm_io
#define _PhiOS_asm_io

#include <stdint.h>

void io_outb(uint16_t a_port, uint8_t a_val);
void io_outw(uint16_t a_port, uint16_t a_val);
void io_outl(uint16_t a_port, uint32_t a_val);
uint8_t io_inb(uint16_t a_port);
uint16_t io_inw(uint16_t a_port);
uint32_t io_inl(uint16_t a_port);
void io_wait();

#endif
