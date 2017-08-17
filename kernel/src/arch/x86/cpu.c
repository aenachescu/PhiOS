#include "include/cpu.h"

#include "util/kstdlib/include/kstdio.h"

void cpu_printState32(
    IntCpuState32 *a_state)
{
    kprintf("--- CPU STATE DUMP---\n");
    kprintf("cr3 = %x cr2 = %x\n", a_state->cr3, a_state->cr2);
    kprintf("gs = %x ds = %x fs = %x es = %x ds = %x\n",
            a_state->gs, a_state->ds, a_state->fs, a_state->es, a_state->ds);
    kprintf("edi = %x esi = %x \nebp = %x ",
            a_state->edi, a_state->esi, a_state->ebp);
    kprintf("kesp = %x\n", a_state->kesp);
    kprintf("edx = %x ecx = %x ebx = %x eax = %x\n",
            a_state->edx, a_state->ecx, a_state->ebx, a_state->eax);
    kprintf("intNo = %x errCode = %x\n", a_state->intNo, a_state->errCode);
    kprintf("eip = %x eflags = %x uesp = %x ss = %x\n",
            a_state->eip, a_state->eflags, a_state->uesp, a_state->ss);
    kprintf("---------------------\n");
}

void cpu_printState64(
    __attribute__((unused)) IntCpuState64 *a_state)
{
    kprintf("--- CPU STATE DUMP---\n");

    kprintf("---------------------\n");
}

void cpu_printState(
    CpuState *a_state)
{
#ifdef PhiOS_ARCH_x86_32
    cpu_printState32(a_state);
#endif // PhiOS_ARCH_x86_32

#ifdef PhiOS_ARCH_x86_64
    cpu_printState64(a_state);
#endif // PhiOS_ARCH_x86_64
}