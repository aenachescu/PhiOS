#include "include/cpu.h"
#include "kernel/include/logging.h"

#if defined PhiOS_ARCH_x86_32

static void cpu_printState32(
    IntCpuState32 *a_state)
{
    KLOG_INFO("--- CPU STATE DUMP---");

    KLOG(
        "eax = %0x, ebx = %0x, ecx = %0x, edx = %0x",
        a_state->eax,
        a_state->ebx,
        a_state->ecx,
        a_state->edx
    );

    KLOG(
        "gs = %0hx, ds = %0hx, fs = %0hx, es = %0hx, ss = %0hx, cs = %0hx",
        a_state->gs,
        a_state->ds,
        a_state->fs,
        a_state->es,
        a_state->ss,
        a_state->cs
    );

    KLOG(
        "cr3 = %0x, cr2 = %0x",
        a_state->cr3,
        a_state->cr2
    );

    KLOG(
        "edi = %0x, esi = %0x",
        a_state->edi,
        a_state->esi
    );

    KLOG(
        "kesp = %0x, uesp = %0x, ebp = %0x, eip = %0x",
        a_state->kesp,
        a_state->uesp,
        a_state->ebp,
        a_state->eip
    );

    KLOG(
        "intNo = %0x, errCode = %0x, eflags = %0x",
        a_state->intNo,
        a_state->errCode,
        a_state->eflags
    );

    KLOG_INFO("---------------------");
}

#elif defined PhiOS_ARCH_x86_64

static void cpu_printState64(
    IntCpuState64 *a_state)
{
    KLOG_INFO("--- CPU STATE DUMP---");
    KLOG_INFO("unsupported, CpuStateAddr: %p", a_state);
    KLOG_INFO("---------------------");
}

#else

#error "unsupported architecture"

#endif

void cpu_printState(
    CpuState *a_state)
{
#if defined PhiOS_ARCH_x86_32
    cpu_printState32(a_state);
#elif defined PhiOS_ARCH_x86_64
    cpu_printState64(a_state);
#else
#error "unsupported architecture"
#endif
}
