#include "kernel/include/arch/x86/init32.h"
#include "kernel/include/arch/x86/cpuid.h"
#include "kernel/include/arch/x86/gdt32.h"
#include "kernel/include/arch/x86/idt32.h"
#include "kernel/include/arch/x86/tss32.h"
#include "kernel/include/arch/x86/pit.h"
#include "kernel/include/arch/x86/pic.h"
#include "kernel/include/arch/x86/paging/ia32.h"
#include "kernel/include/arch/x86/paging/paging.h"

#include "kernel/include/memory/paa.h"
#include "kernel/include/memory/pmm.h"
#include "kernel/include/memory/bitmap_pma.h"

#include "drivers/video/include/vga/text_mode.h"
#include "drivers/keyboard/include/keyboard.h"
#include "drivers/rtc/include/rtc.h"

#include "util/kstdlib/include/kstdio.h"

#include "kernel/include/multiboot2.h"

struct KernelArea g_kernelArea;

extern uint32 linker_kernelStart;
extern uint32 linker_kernelEnd;
extern uint32 linker_textStart;
extern uint32 linker_textEnd;
extern uint32 linker_rodataStart;
extern uint32 linker_rodataEnd;
extern uint32 linker_dataStart;
extern uint32 linker_dataEnd;
extern uint32 linker_bssStart;
extern uint32 linker_bssEnd;

#define FRAME_SIZE 4096

// TODO: remove this when tasks are available
size_t g_kernelStack[2048]; // temporary kernel mode

struct BitmapPMA g_PMAVM; // physical memory allocator for virtual memory
struct Paging g_kernelPaging;

extern struct PMA *g_allocators;

size_t init_init32(uint32 mboot2Magic, uint32 mboot2Addr)
{
    // Inits VGA
    VGA_Init();
    kprintf("PhiOS v0.0.1 32-bit\n");

    if (mboot2Magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        VGA_WriteString("[PANIC] GRUB not multiboot2");
        return ERROR_NOT_FOUND;
    }

    if (mboot2Addr & 7)
    {
        VGA_WriteString("[PANIC] Multiboot2 structure is not aligned");
        return ERROR_UNALIGNED_ADDRESS;
    }

    VGA_WriteString("GRUB multiboot2\n");

    // Inits Placement Address Allocator
    PAA_init((size_t) &linker_kernelEnd);

    // Inits CPUID detection
    CPUID_Init();
    const char *cpuVendorName = NULL;
    CPUID_GetVendorName(&cpuVendorName);
    kprintf("[CPU] %s\n", cpuVendorName);

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
                    &BitmapPMA_alloc, &BitmapPMA_free, &BitmapPMA_reserve);
    PMM_reserve(0x0, 0x100000, PMM_FOR_VIRTUAL_MEMORY);

    kprintf("KernelStart: %x KernelEnd: %x\n",
        &linker_kernelStart, &linker_kernelEnd);

    kprintf("TextStart: %x TextEnd: %x\n",
        &linker_textStart, &linker_textEnd);

    kprintf("RodataStart: %x RodataEnd: %x\n",
        &linker_rodataStart, &linker_rodataEnd);

    kprintf("DataStart: %x DataEnd: %x\n",
        &linker_dataStart, &linker_dataEnd);

    kprintf("BssStart: %x BssEnd: %x\n",
        &linker_bssStart, &linker_bssEnd);

    kprintf("Placement address: %x\n", PAA_getCurrentAddress());

    g_kernelArea.textStartAddr      = (size_t) &linker_textStart;
    g_kernelArea.textEndAddr        = (size_t) &linker_textEnd;
    g_kernelArea.rodataStartAddr    = (size_t) &linker_rodataStart;
    g_kernelArea.rodataEndAddr      = (size_t) &linker_rodataEnd;
    g_kernelArea.dataStartAddr      = (size_t) &linker_dataStart;
    g_kernelArea.dataEndAddr        = (size_t) &linker_dataEnd;
    g_kernelArea.bssStartAddr       = (size_t) &linker_bssStart;
    g_kernelArea.bssEndAddr         = (size_t) &linker_bssEnd;
    g_kernelArea.endPlacementAddr   = PAA_getCurrentAddress();

    PMM_reserve(g_kernelArea.textStartAddr,
                g_kernelArea.endPlacementAddr - g_kernelArea.textStartAddr,
               PMM_FOR_VIRTUAL_MEMORY);
    IA32_4KB_initKernelPaging(&g_kernelPaging);

    g_PMAVM.bitmap = (size_t*) ((size_t)g_PMAVM.bitmap + 0xC0000000 - 0x00100000);
    g_allocators = (struct PMA*) ((size_t)g_allocators + 0xC0000000 - 0x00100000);

    IA32_4KB_switchDirectory(&g_kernelPaging,
        (struct IA32_PageDirectory_4KB*) g_kernelPaging.pagingStruct);

    return ERROR_SUCCESS;
}