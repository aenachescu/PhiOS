#include "kernel/include/arch/x86/gdt32.h"
#include "kernel/include/arch/x86/idt32.h"
#include "kernel/include/arch/x86/tss32.h"
#include "kernel/include/arch/x86/pit.h"
#include "kernel/include/arch/x86/pic.h"
#include "kernel/include/arch/x86/cpuid.h"
#include "kernel/include/memory/pmm.h"

#include "drivers/keyboard/include/keyboard.h"
#include "drivers/rtc/include/rtc.h"

#include "util/kstdlib/include/kstdio.h"

extern size_t g_kernelStack[2048];

// TODO: remove this when tasks are available
size_t g_userStack[2048]; // temporary user mode

void user_main()
{
    kprintf("Hello, world!\n");
    kprintf("> ");
    while (1) {
        char c = keyboard_readKey();
        if (c == '\n') {
            kprintf("\n> ");
        }

        if (c == 'q') {
            break;
        }
        else {
            kprintf("%c", c);
        }
    }
}

void kernel_main()
{
    kprintf("paging enabled\n");

    // Inits CPUID detection
    KERNEL_CHECK(CPUID_Init());
    const char *cpuVendorName = NULL;
    CPUID_GetVendorName(&cpuVendorName);
    kprintf("[CPU] %s\n", cpuVendorName);

    // Inits real time clock
    KERNEL_CHECK(RTC_init());
    kprintf("[SYSTEM] Initialized real time clock.\n");

    // Inits GDT for 32-bit
    KERNEL_CHECK(GDT32_init());

    // Sets kernel stack in TSS struct
    KERNEL_CHECK(TSS32_setKernelStack((uint32) &g_kernelStack[2047]));

    // Inits IDT for 32-bit
    KERNEL_CHECK(IDT32_init());

    // Inits PIC
    KERNEL_CHECK(PIC_init());
    KERNEL_CHECK(PIC_maskUnusedIRQs());

    // Inits timer
    KERNEL_CHECK(PIT_init((uint16) -1));
    kprintf("[SYSTEM] Initialized timer at %d frequency.\n", OSCILLATOR_FREQUENCY);

    // Inits keyboard
    KERNEL_CHECK(keyboard_init());
    kprintf("[SYSTEM] Initialized keyboard.\n");

    return;
}
