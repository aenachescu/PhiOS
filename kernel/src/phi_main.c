#include "vga/text_mode.h"
#include "types.h"
#include "memory/paa.h"
#include "memory/pmm.h"
#include "memory/bitmap_pma.h"
#include "kstdio.h"
#include "arch/x86/cpuid.h"
#include "arch/x86/gdt32.h"
#include "arch/x86/idt32.h"
#include "arch/x86/tss32.h"
#include "arch/x86/pit.h"
#include "arch/x86/pic.h"
#include "keyboard.h"
#include "rtc.h"
#include "arch/x86/paging/paging.h"

#include "multiboot2.h"

struct KernelArea g_kernelArea;

extern size_t linker_kernelStart;
extern size_t linker_kernelEnd;

#define PRINT(x) kprintf("%s - %u\n", #x, CPUID_HasFeature(x))
#define FRAME_SIZE 4096

extern void jumpToUserMode();
// THIS IS TEMPORARY
// TODO: remove this when tasks are available
size_t g_userStack[2048];   // temporary user mode
size_t g_kernelStack[2048]; // temporary kernel mode

char *g_CPUvendorName;
struct BitmapPMA g_PMAVM; // physical memory allocator for virtual memory

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

#ifdef PhiOS_ARCH_x86_32
    // Inits CPUID detection
    CPUID_Init();
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

    // Inits Physical Memory Manager
    PMM_init(1);
    // Temporary BitmapPMA that uses memory from 0 to 4 MiB
    // It should use memory from 0 to end of RAM
    BitmapPMA_createAllocator(&g_PMAVM, FRAME_SIZE, 0x0, 0x400000);
    PMM_addAllocator((void*) &g_PMAVM, PMM_FOR_VIRTUAL_MEMORY,
                    &BitmapPMA_alloc, &BitmapPMA_free);
    size_t addr1, addr2, addr3;
    PMM_alloc(&addr1, 2 * FRAME_SIZE, PMM_FOR_VIRTUAL_MEMORY);
    PMM_alloc(&addr2, 4 * FRAME_SIZE, PMM_FOR_VIRTUAL_MEMORY);
    kprintf("Alloc: %x %x\n", addr1, addr2);
    PMM_free(addr1, 2 * FRAME_SIZE, PMM_FOR_VIRTUAL_MEMORY);
    kprintf("Free: %x\n", addr1);
    PMM_alloc(&addr3, 2 * FRAME_SIZE, PMM_FOR_VIRTUAL_MEMORY);
    kprintf("Alloc: %x\n", addr3);
    PMM_alloc(&addr1, 10 * FRAME_SIZE, PMM_FOR_VIRTUAL_MEMORY);
    kprintf("Alloc: %x\n", addr1);

    // Go to user mode
    jumpToUserMode();
#endif // PhiOS_ARCH_x86_32

    return ;
}
