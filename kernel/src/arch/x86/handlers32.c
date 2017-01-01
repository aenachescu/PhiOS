#include "arch/x86/idt32.h"
#include "arch/x86/handlers32.h"
#include "kstdio.h"
#include "cpu.h"

void handlers32_default(IntCpuState32 *a_state)
{
    kprintf("--- KERNEL PANIC ---\n");
    kprintf("This interrupt is not supported.\n");
    kprintf("cr3 = %u cr2 = %u\n", a_state->cr3, a_state->cr2);
    kprintf("gs = %u ds = %u fs = %u es = %u ds = %u\n",
            a_state->gs, a_state->ds, a_state->fs, a_state->es, a_state->ds);
    kprintf("edi = %u esi = %u \nebp = %u ",
            a_state->edi, a_state->esi, a_state->ebp);
    kprintf("kesp = %u\n", a_state->kesp);
    kprintf("edx = %u ecx = %u ebx = %u eax = %u\n",
            a_state->edx, a_state->ecx, a_state->ebx, a_state->eax);
    kprintf("intNo = %u errCode = %d\n", a_state->intNo, a_state->errCode);
    kprintf("eip = %u eflags = %u uesp = %u ss = %u\n",
            a_state->eip, a_state->eflags, a_state->uesp, a_state->ss);
    kprintf("--------------------\n");
    stopCpu();
}
