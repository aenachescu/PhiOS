#include "kernel/include/arch/x86/handlers32.h"
#include "include/cpu.h"
#include "drivers/video/include/vga/text_mode.h"

void handlers32_default(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("PANIC", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Interrupt is not supported.\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_zeroDivision(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Division by zero...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_debug(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Debugging...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_NMI(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Non-maskable interrupt...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_breakpoint(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("BREAKPOINT.\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_overflow(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Memory overflow...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_boundRangeExceed(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Bound range exceeded...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_invaildOpcode(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Invalid opcode...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_deviceNotAvailable(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Device is not available...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_doubleFault(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Double fault...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_coprocessorSegmentOverrun(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Coprocessor segment overrun...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_invalidTSS(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("TSS is invalid...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_segmentNotPresent(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Segment is not present...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_stackSegmentFault(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Stack segment fault...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_generalProtectionFault(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("General protection fault occured...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_pageFault(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Page fault...\n");
    cpu_printState32(a_state);
    
    // Specific information about the memory
    VGA_WriteString("[");
    VGA_WriteColoredString("ADDRESS", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    kprintf("%x\n", a_state->cr2);
#define CHECK_BIT(var, pos) ((var) & (1 << pos))
    VGA_WriteString("[");
    VGA_WriteColoredString("BITS", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");

    /*
     * first colum: value to print if the bit is set
     * second column: value to print if the bit is not set
     */
    const char *status[][2] = {
        "P ",   "NP ",
        "W ",   "R ",
        "U ",   "S ",
        "RSVD ", "\0",
        "I ",   "D " 
    };
    const uint32 cases = 5;

    for (uint32 i = 0; i < cases; i++) {
        if (CHECK_BIT(a_state->errCode, i)) {
            VGA_WriteString(status[i][0]);
        } else {
            VGA_WriteString(status[i][1]);
        }
    }
#undef CHECK_BIT

    stopCpu();
}

void handlers32_FPUx87Exception(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Floating point fault...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_alignmentCheck(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Alignment check fail...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_machineCheck(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Machine check fail...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_SIMDFloatingPoint(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("SIMD Floating Point error...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_virtualization(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Virtualization problem...\n");
    cpu_printState32(a_state);
    stopCpu();
}

void handlers32_security(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("EXCEPTION", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("] ");
    VGA_WriteString("Security fault...\n");
    cpu_printState32(a_state);
    stopCpu();
}
