#include "vga/text_mode.h"
#include "types.h"
#include "memory/paa.h"
#include "kstdio.h"
#include "arch/x86/cpuid.h"
#include "arch/x86/gdt32.h"
#include "arch/x86/idt32.h"
#include "arch/x86/pit.h"
#include "arch/x86/pic.h"

#include "multiboot2.h"

#define PRINT(x) kprintf("%s - %u\n", #x, CPUID_HasFeature(x))

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

    PAA_Init();
    GDT_init32();
    IDT_init32();
    PIC_init();
    PIC_maskUnusedIRQs();
    PIT_init((uint16) -1);

    turnOnInts();

    return ;
}
