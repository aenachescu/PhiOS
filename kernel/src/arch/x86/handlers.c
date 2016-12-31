#include "arch/x86/idt32.h"
#include "arch/x86/idt64.h"
#include "arch/x86/handlers.h"
#include "kstdio.h"
#include "cpu.h"

void standardHandler32(IntCpuState32 *a_state)
{
    // TODO = %d print all registers
    kprintf("--- KERNEL PANIC ---\n");
    kprintf("This interrupt is not supported.\n");
    kprintf("cr3 = %d cr2 = %d\n");
    kprintf("gs = %d ds = %d fs = %d es = %d ds = %d\n");
    kprintf("edi = %d esi = %d \nebp = %d ");
    kprintf("kesp = %d\n");
    kprintf("edx = %d \necx = %d \nebx = %d \neax = %d\n");
    kprintf("intNo = %d errCode = %d\n");
    kprintf("eip = %d eflags = %d \nuesp = %d ss = %d\n");
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
