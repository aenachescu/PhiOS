#include "types.h"
#include "kernel/include/arch/x86/pic.h"
#include "kernel/include/arch/x86/asm_io.h"
#include "errors.h"

uint32 PIC_init()
{
    io_outb(PIC1_COMMAND, 0x11);
    io_outb(PIC2_COMMAND, 0x11);
    io_outb(PIC1_DATA, PIC1_COMMAND);
    io_outb(PIC2_DATA, 0x28);
    io_outb(PIC1_DATA, 0x04);
    io_outb(PIC2_DATA, 0x02);
    io_outb(PIC1_DATA, 0x01);
    io_outb(PIC2_DATA, 0x01);
    io_outb(PIC1_DATA, 0x0);
    io_outb(PIC2_DATA, 0x0);

    return ERROR_SUCCESS;
}

uint32 PIC_setMask(
    uint8 a_irqLine)
{
    uint16 port;
    uint8 value;

    if (a_irqLine < 8) {
        port = PIC1_DATA;
    }
    else {
        port = PIC2_DATA;
        a_irqLine -= 8;
    }

    value = io_inb(port) | (1 << a_irqLine);
    io_outb(port, value);

    return ERROR_SUCCESS;
}

uint32 PIC_clearMask(
    uint8 a_irqLine)
{
    uint16 port;
    uint8 value;

    if (a_irqLine < 8) {
        port = PIC1_DATA;
    }
    else {
        port = PIC2_DATA;
        a_irqLine -= 8;
    }

    value = io_inb(port) & ~(1 << a_irqLine);
    io_outb(port, value);

    return ERROR_SUCCESS;
}

uint32 PIC_maskUnusedIRQs()
{
    for (uint8 i = 2; i < 16; i++) {
        PIC_setMask(i);
    }

    return ERROR_SUCCESS;
}
