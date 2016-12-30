#include "arch/x86/idt.h"
#include "arch/x86/handlers.h"
#include "kstdio.h"
#include "cpu.h"

void standardHandler32(IntCpuState32 *a_state)
{
    // TODO: print all registers
    kprintf("--- KERNEL PANIC ---\n");
    kprintf("This interrupt is not supported.\n");
    kprintf("--------------------\n");
    stopCpu();
}

void standardHandler64(IntCpuState64 *a_state)
{
    kprintf("--- KERNEL PANIC ---\n");
    kprintf("This interrupt is not supported.\n");
    kprintf("--------------------\n");
    stopCpu();
}
