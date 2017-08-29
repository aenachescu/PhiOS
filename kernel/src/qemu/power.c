#include "kernel/include/arch/x86/asm_io.h"
#include "kernel/include/qemu/power.h"
#include "cpu.h"
#include "types.h"

void qemu_reboot()
{
    uint8 data = 0x02;
    while (data & 0x02)
        data = io_inb(0x64);
    io_outb(0x64, 0xFE);
    pauseCpu();
}

void qemu_shutdown()
{
    io_outb(0xf4, 0x00);
}