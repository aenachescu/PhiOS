#ifndef PhiOS_cpu
#define PhiOS_cpu

#include "types.h"

#if defined PhiOS_ARCH_x86_32 || defined PhiOS_ARCH_x86_64

// Set interrupts bit to 1
#define turnOnInts() asm volatile("sti")

// Set interrupts bit to 0
#define turnOffInts() asm volatile("cli")

// Pause CPU until next interrupt occurs
#define pauseCpu() asm volatile("hlt")

// Stop CPU execution and turn off interrupts
#define stopCpu() turnOffInts(); while(true) { pauseCpu(); }

// Stop CPU exectuion, but interrupts remain turned on
#define freezeCpu() while(true) { pauseCpu(); }

typedef struct IntCpuState32 {
    uint32 cr3;
    uint32 cr2;
    uint16 gs;
    uint16 fs;
    uint16 es;
    uint16 ds;
    uint32 edi;
    uint32 esi;
    uint32 ebp;
    uint32 kesp;
    uint32 edx;
    uint32 ecx;
    uint32 ebx;
    uint32 eax;
    uint32 intNo;
    uint32 errCode;
    uint32 eip;
    uint16 cs;
    uint32 eflags;
    uint32 uesp;
    uint16 ss;
} IntCpuState32;

#ifdef PhiOS_ARCH_x86_32

typedef IntCpuState32 CpuState;

#endif // PhiOS_ARCH_x86_32

typedef struct IntCpuState64 {
    uint64 cr3;
    uint64 cr2;
    uint16 gs;
    uint16 fs;
    uint16 es;
    uint16 ds;
    uint64 rdi;
    uint64 rsi;
    uint64 rbp;
    uint64 krsp;
    uint64 r15;
    uint64 r14;
    uint64 r13;
    uint64 r12;
    uint64 r11;
    uint64 r10;
    uint64 r9;
    uint64 r8;
    uint64 rdx;
    uint64 rcx;
    uint64 rbx;
    uint64 rax;
    uint64 intNo;
    uint64 errCode;
    uint64 rip;
    uint16 cs;
    uint64 rflags;
    uint64 ursp;
    uint16 ss;
} IntCpuState64;

#ifdef PhiOS_ARCH_x86_64

typedef IntCpuState64 CpuState;

#endif // PhiOS_ARCH_x86_64

void cpu_printState32(
    IntCpuState32 *
);

void cpu_printState64(
    IntCpuState64 *a_state
);

void cpu_printState(
    CpuState *a_state
);

#endif // PhiOS_ARCH_x86_32 || PhiOS_ARCH_x86_64

#endif // PhiOS_cpu
