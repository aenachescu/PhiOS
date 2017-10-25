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
#include "kernel/include/logging.h"
extern struct Paging g_kernelPaging;

#include "drivers/keyboard/include/keyboard.h"
#include "drivers/rtc/include/rtc.h"
#include "drivers/video/include/vga/text_mode.h"

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
            VGA_Focus();
            kprintf("\n> ");
            continue;
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
        } else if (c == KUP) {
            VGA_ScreenScrollUp(1);
        } else if (c == KDOWN) {
            VGA_ScreenScrollDown(1);
        } else if (c == KPGUP) {
            VGA_ScreenScrollUp(VGA_HEIGHT);
        } else if (c == KPGDN) {
            VGA_ScreenScrollDown(VGA_HEIGHT);
        } else {
            VGA_Focus();
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

    struct VirtualAllocRequest request;

    request.flags = PAGING_ALLOC_FLAG_AT_ADDRESS;
    request.pageFlags = PAGING_ALLOC_PAGE_FLAG_WRITE;
    request.virtualAddress = 0x00001000;
    request.length = 0x1000;
    request.physicalAddress = 0x0;

    uint64 addr;
    uint32 err = IA32_4KB_alloc(&g_kernelPaging, &request, &addr);

    KLOG("ptr: %llx", addr);
    KLOG("err: %u", err);
    uint32 *ptr = (uint32*)((uint32)addr);
    ptr[0] = 10;
    KLOG("ptr val: %u", ptr[0]);

    return;
}
