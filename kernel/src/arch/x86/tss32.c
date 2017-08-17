#include "kernel/include/arch/x86/tss32.h"
#include "util/kstdlib/include/kstring.h"

struct TSS32_Entry g_TSSKernelEntry;

uint32 TSS32_init(uint16 a_ss0, uint32 a_esp0)
{
    kmemset(&g_TSSKernelEntry, 0, sizeof(struct TSS32_Entry));

    g_TSSKernelEntry.ss0 = a_ss0;
    g_TSSKernelEntry.esp0 = a_esp0;

    g_TSSKernelEntry.cs = 0x0b;
    g_TSSKernelEntry.ss = 0x13;
    g_TSSKernelEntry.ds = 0x13;
    g_TSSKernelEntry.es = 0x13;
    g_TSSKernelEntry.fs = 0x13;
    g_TSSKernelEntry.gs = 0x13;

    return ERROR_SUCCESS;
}

uint32 TSS32_setKernelStack(uint32 a_esp)
{
    g_TSSKernelEntry.esp0 = a_esp;

    return ERROR_SUCCESS;
}
