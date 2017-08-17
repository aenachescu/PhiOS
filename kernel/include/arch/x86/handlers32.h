#ifndef PhiOS_exception_handlers32
#define PhiOS_exception_handlers32

#include "kernel/include/arch/x86/idt32.h"
#include "include/cpu.h"

void handlers32_default(IntCpuState32 *a_state);
void handlers32_zeroDivision(IntCpuState32 *a_state);
void handlers32_debug(IntCpuState32 *a_state);
void handlers32_NMI(IntCpuState32 *a_state);
void handlers32_breakpoint(IntCpuState32 *a_state);
void handlers32_overflow(IntCpuState32 *a_state);
void handlers32_boundRangeExceed(IntCpuState32 *a_state);
void handlers32_invaildOpcode(IntCpuState32 *a_state);
void handlers32_deviceNotAvailable(IntCpuState32 *a_state);
void handlers32_doubleFault(IntCpuState32 *a_state);
void handlers32_coprocessorSegmentOverrun(IntCpuState32 *a_state);
void handlers32_invalidTSS(IntCpuState32 *a_state);
void handlers32_segmentNotPresent(IntCpuState32 *a_state);
void handlers32_stackSegmentFault(IntCpuState32 *a_state);
void handlers32_generalProtectionFault(IntCpuState32 *a_state);
void handlers32_pageFault(IntCpuState32 *a_state);
void handlers32_FPUx87Exception(IntCpuState32 *a_state);
void handlers32_alignmentCheck(IntCpuState32 *a_state);
void handlers32_machineCheck(IntCpuState32 *a_state);
void handlers32_SIMDFloatingPoint(IntCpuState32 *a_state);
void handlers32_virtualization(IntCpuState32 *a_state);
void handlers32_security(IntCpuState32 *a_state);


#endif
