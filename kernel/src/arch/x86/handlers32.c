#include "kernel/include/arch/x86/handlers32.h"
#include "include/cpu.h"
#include "kernel/include/logging.h"

void handlers32_default(IntCpuState32 *a_state)
{
    KLOG_FATAL("[PANIC] interrupt is not supported");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_zeroDivision(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] division by zero");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_debug(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Debugging");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_NMI(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Non-maskable interrupt");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_breakpoint(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] breakpoint");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_overflow(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Memory overflow");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_boundRangeExceed(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Bound range exceeded");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_invaildOpcode(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Invalid opcode");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_deviceNotAvailable(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Device is not available");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_doubleFault(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Double fault");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_coprocessorSegmentOverrun(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Coprocessor segment overrun");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_invalidTSS(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] TSS is invalid");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_segmentNotPresent(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Segment is not present");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_stackSegmentFault(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Stack segment fault");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_generalProtectionFault(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] General protection fault occured");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_pageFault(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] page fault");
    cpu_printState(a_state);
    
    // Specific information about the memory
    KLOG_INFO("Faulting address is: %p", a_state->cr2);
#define CHECK_BIT(var, pos) ((var) & (1 << pos))
    KLOG_INFO("The fault was caused by:");

    /*
     * first colum: value to print if the bit is set
     * second column: value to print if the bit is not set
     */
    const char *status[][2] = {
        {"- breaking protection rules",                        "- a not present page"},
        {"- a writing action",                                 "- a reading action"},
        {"- an user space instruction",                        "- a kernel space instruction"},
        {"- reserved bits in page",                            "- unknown1"},
        {"- an instruction fetch (no-execute bit is enabled)", "- unknown2"}
    };
    const uint32 cases = 5;

    for (uint32 i = 0; i < cases; i++) {
        if (CHECK_BIT(a_state->errCode, i)) {
            KLOG_INFO("%s", status[i][0]);
        } else {
            KLOG_INFO("%s", status[i][1]);
        }
    }
#undef CHECK_BIT

    stopCpu();
}

void handlers32_FPUx87Exception(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Floating point fault");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_alignmentCheck(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Alignment check fail");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_machineCheck(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] Machine check fail");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_SIMDFloatingPoint(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] SIMD Floating Point error");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_virtualization(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] virtualization");
    cpu_printState(a_state);
    stopCpu();
}

void handlers32_security(IntCpuState32 *a_state)
{
    KLOG_FATAL("[EXCEPTION] security fault");
    cpu_printState(a_state);
    stopCpu();
}
