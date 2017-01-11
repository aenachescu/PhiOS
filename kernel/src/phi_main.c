#include "vga/text_mode.h"
#include "types.h"
#include "memory/paa.h"
#include "kstdio.h"
#include "arch/x86/cpuid.h"
#include "arch/x86/gdt32.h"
#include "arch/x86/idt32.h"
#include "arch/x86/tss32.h"
#include "arch/x86/pit.h"
#include "arch/x86/pic.h"

#include "multiboot2.h"

extern size_t linker_kernelStart;
extern size_t linker_kernelEnd;

#define PRINT(x) kprintf("%s - %u\n", #x, CPUID_HasFeature(x))

extern void jumpToUserMode();
// THIS IS TEMPORARY
// TODO: remove this when tasks are available
size_t g_userStack[2048];   // temporary user mode
size_t g_kernelStack[2048]; // temporary kernel mode

void user_main()
{
    kprintf("Hello, world!\n");
    while (1) {}
}

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

    // Inits CPUID detection
    CPUID_Init();

    // Inits Placement Address Allocator
    PAA_init((size_t) &linker_kernelEnd);

    // Inits GDT for 32-bit
    GDT32_init();

    // Sets kernel stack in TSS struct
    TSS32_setKernelStack((uint32) &g_kernelStack[2047]);

    // Inits IDT for 32-bit
    IDT32_init();

    // Inits PIC
    PIC_init();
    PIC_maskUnusedIRQs();

    // Inits timer
    PIT_init((uint16) -1);

    // Go to user mode
    jumpToUserMode();

    return ;
}
