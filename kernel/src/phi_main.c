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
#include "keyboard.h"
#include "rtc.h"

#include "multiboot2.h"

extern size_t linker_kernelStart;
extern size_t linker_kernelEnd;

#define PRINT(x) kprintf("%s - %u\n", #x, CPUID_HasFeature(x))

extern void jumpToUserMode();
// THIS IS TEMPORARY
// TODO: remove this when tasks are available
size_t g_userStack[2048];   // temporary user mode
size_t g_kernelStack[2048]; // temporary kernel mode

char *g_CPUvendorName;

void user_main()
{
    kprintf("Hello, world!\n");
    kprintf("> ");
    while (1)
    {
        char c = keyboard_readKey();
        if (c == '\n')
        {
            kprintf("\n> ");
        }
        else
        {
            kprintf("%c", c);
        }
    }
}

void kernel_main(unsigned long magic, size_t addr)
{
    // Inits VGA
    VGA_Init();
    kprintf("PhiOS v0.0.1 ");

#if defined WORDSIZE && WORDSIZE == 32
    VGA_WriteString("32-bit ");
#endif

#if defined WORDSIZE && WORDSIZE == 64
    VGA_WriteString("64-bit ");
#endif

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        VGA_WriteString("\n[PANIC] GRUB not multiboot2");
        return;
    }

    VGA_WriteString("GRUB multiboot2\n");

    if (addr & 7)
    {
        VGA_WriteString("[PANIC] Multiboot2 structure is not aligned");
        return;
    }

    // Inits Placement Address Allocator
    PAA_init((size_t) &linker_kernelEnd);

    // Inits CPUID detection
    CPUID_Init();
    PAA_alloc(256, (size_t*) g_CPUvendorName, 1);
    CPUID_GetVendorName((const char**) &g_CPUvendorName);
    kprintf("[CPU] %s \n", g_CPUvendorName);

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
    kprintf("[SYSTEM] Initialized timer at %d frequency.\n", OSCILLATOR_FREQUENCY);

    // Inits keyboard
    keyboard_init();
    kprintf("[SYSTEM] Initialized keyboard.\n");

    // Inits real time clock
    RTC_init();
    kprintf("[SYSTEM] Initialized real time clock.\n");

    // Go to user mode
    jumpToUserMode();

    return ;
}
