#include "vga/text_mode.h"
#include "types.h"
#include "memory/paa.h"
#include "kstdio.h"
#include "arch/x86/cpuid.h"

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

    int *p1 = 0;
    int *p2 = 0;
    char *c1 = 0;
    char *c2 = 0;

    PAA_Alloc(sizeof(int) * 5, (size_t*) &p1);
    PAA_Alloc(sizeof(int) * 10, (size_t*) &p2);
    PAA_Alloc(sizeof(char) * 5, (size_t*) &c1);
    PAA_Alloc(sizeof(char) * 15, (size_t*) &c2);

    kprintf("%u\n%u\n%u\n%u\n", p1, p2, c1, c2);

    size_t error = CPUID_Init();
    kprintf("error: %u\n", error);

    PRINT(CPU_FEATURE_FPU);
    PRINT(CPU_FEATURE_MMX);
    PRINT(CPU_FEATURE_SSE);
    PRINT(CPU_FEATURE_SSE2);
    PRINT(CPU_FEATURE_SSE3);
    PRINT(CPU_FEATURE_SSE4_1);
    PRINT(CPU_FEATURE_SSE4_2);
    PRINT(CPU_FEATURE_PAE);
    PRINT(CPU_FEATURE_PSE);
    PRINT(CPU_FEATURE_APIC);
    PRINT(CPU_FEATURE_SEP);
    PRINT(CPU_FEATURE_MTRR);
    PRINT(CPU_FEATURE_PSE_36);
    const char *vendor = NULL;
    CPUID_GetVendorName(&vendor);
    kprintf("Vendor: %s\n", vendor);

    return ;
}
