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
#include "arch/x86/paging/ia32.h"
#include "keyboard.h"
#include "rtc.h"
#include "arch/x86/paging/paging.h"

#include "multiboot2.h"

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

#define PRINT(x) kprintf("%s - %u\n", #x, CPUID_HasFeature(x))
#define FRAME_SIZE 4096

extern void jumpToUserMode();
// THIS IS TEMPORARY
// TODO: remove this when tasks are available
size_t g_userStack[2048];   // temporary user mode
size_t g_kernelStack[2048]; // temporary kernel mode

char *g_CPUvendorName;
struct BitmapPMA g_PMAVM; // physical memory allocator for virtual memory
struct Paging g_kernelPaging;

extern struct PMA *g_allocators;

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

    kprintf("Placement address: %u\n", PAA_getCurrentAddress());

    g_kernelArea.textStartAddr = &linker_textStart;
    g_kernelArea.textEndAddr = &linker_textEnd;
    g_kernelArea.rodataStartAddr = &linker_rodataStart;
    g_kernelArea.rodataEndAddr = &linker_rodataEnd;
    g_kernelArea.dataStartAddr = &linker_dataStart;
    g_kernelArea.dataEndAddr = &linker_dataEnd;
    g_kernelArea.bssStartAddr = &linker_bssStart;
    g_kernelArea.bssEndAddr = &linker_bssEnd;
    g_kernelArea.endPlacementAddr = PAA_getCurrentAddress();

    PMM_reserve(g_kernelArea.textStartAddr,
                g_kernelArea.endPlacementAddr - g_kernelArea.textStartAddr,
               PMM_FOR_VIRTUAL_MEMORY);
    IA32_4KB_initKernelPaging(&g_kernelPaging);

    g_PMAVM.bitmap = (size_t*) ((size_t)g_PMAVM.bitmap + 0xC0000000 - 0x00100000);
    g_allocators = (struct PMA*) ((size_t)g_allocators + 0xC0000000 - 0x00100000);

    //enable paging

    kprintf("paging enabled\n");

    // Go to user mode
    //jumpToUserMode();
#endif // PhiOS_ARCH_x86_32

    return ;
}
