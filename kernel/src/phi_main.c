#include "kernel/include/arch/x86/gdt32.h"
#include "kernel/include/arch/x86/idt32.h"
#include "kernel/include/arch/x86/tss32.h"
#include "kernel/include/arch/x86/pit.h"
#include "kernel/include/arch/x86/pic.h"
#include "kernel/include/arch/x86/cpuid.h"
#include "kernel/include/memory/pmm.h"
#include "kernel/include/qemu/power.h"
#include "kernel/include/arch/x86/paging/ia32.h"
#include "kernel/include/arch/x86/paging/paging.h"
extern struct Paging g_kernelPaging;

#include "drivers/keyboard/include/keyboard.h"
#include "drivers/rtc/include/rtc.h"
#include "drivers/serial/include/serial.h"

#include "util/kstdlib/include/kstdio.h"

extern size_t g_kernelStack[2048];

// TODO: remove this when tasks are available
size_t g_userStack[2048]; // temporary user mode

void user_main()
{
    kprintf("Hello, world!\n");
    kprintf("> ");
    while (1) {
        uint8 c = keyboard_readKey();
        if (c == '\n') {
            kprintf("\n> ");
        }

        if (c == ESC) {
            kprintf("\nCommand line exited!\n");
            break;
        } else if (c == KF1) {
            kprintf("\nReboot will work only on QEMU for now...\n");
            qemu_reboot();
        } else if (c == KF2) {
            qemu_shutdown();
            kprintf("\nShutdown will work only on QEMU for now...\n");
        } else {
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

    serial_init();
    serial_writeString("oh, serial is working :o\n", SERIAL_PORT_A);
/*
    struct AllocFuncParam request;
    struct IA32_4KB_Paging_AllocParam ia32_request;

    request.pagingType = PAGING_TYPE_IA32_4KB;
    request.param = &ia32_request;

    ia32_request.flag = PAGING_FLAG_ALLOC_AT_ADDRESS;
    ia32_request.user = false;
    ia32_request.write = true;
    ia32_request.cacheDisabled = false;
    ia32_request.writeThrough = false;
    ia32_request.virtualAddress = 0xBFFFFFFF;
    ia32_request.length = 0x1000;
    ia32_request.physicalAddress = 0;

    uint32 addr;
    KERNEL_CHECK(IA32_4KB_alloc(&g_kernelPaging, &request, &addr));

    freezeCpu();
*/
    return;
}
