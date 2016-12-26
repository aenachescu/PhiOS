#include "vga/text_mode.h"
#include "types.h"

#include "multiboot2.h"

void kernel_main(unsigned long magic, size_t addr)
{
    VGA_Init();
    VGA_WriteString("Hello, PhiOS!\n");
    VGA_WriteString("Halt\n");

#if defined WORDSIZE && WORDSIZE == 32
    VGA_WriteString("32\n");
#endif

#if defined WORDSIZE && WORDSIZE == 64
    VGA_WriteString("64\n");
#endif

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        VGA_WriteString("Is not multiboot2\n");
        return;
    }

    VGA_WriteString("Is multiboot2\n");

    if (addr & 7)
    {
        VGA_WriteString("Multiboot2 structure is not aligned");
        return;
    }

    return ;
}
