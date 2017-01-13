#include "types.h"
#include "arch/x86/idt32.h"
#include "arch/x86/asm_io.h"
#include "arch/x86/handlers32.h"
#include "arch/x86/pic.h"
#include "errors.h"
#include "cpu.h"
#include "kstring.h"
#include "keyboard.h"

static IDT32_Entry g_IDTEntries32[IDT_ENTRIES];
static IDT32 g_IDTPointer32;
static ISR32_PFN g_intHandlers[IDT_ENTRIES];

extern void IDT32_Load(uint32 a_table);

static size_t helper_setEntry32(uint32 a_index, uint32 a_base,
                                uint16 a_selector, uint8 a_flags)
{
    if (a_index >= IDT_ENTRIES)
    {
        return ERROR_INVALID_PARAMETER;
    }

    g_IDTEntries32[a_index].lowOffset   = a_base & 0xFFFF;
    g_IDTEntries32[a_index].highOffset  = (a_base >> 16) & 0xFFFF;
    g_IDTEntries32[a_index].selector    = a_selector;
    g_IDTEntries32[a_index].alwaysZero  = 0;
    g_IDTEntries32[a_index].type        = a_flags;

    return ERROR_SUCCESS;
}

size_t IDT32_init()
{
    g_IDTPointer32.limit = sizeof(IDT32_Entry) * IDT_ENTRIES - 1;
    g_IDTPointer32.base = (uint32) &g_IDTEntries32;

    kmemset(&g_IDTEntries32, 0, sizeof(IDT32_Entry) * IDT_ENTRIES);

    for (uint32 i = 0; i < IDT_ENTRIES; i++)
    {
        IDT32_registerHandler(i, &handlers32_default);
    }

    IDT32_registerHandler(0, &handlers32_zeroDivision);
    IDT32_registerHandler(1, &handlers32_debug);
    IDT32_registerHandler(2, &handlers32_NMI);
    IDT32_registerHandler(3, &handlers32_breakpoint);
    IDT32_registerHandler(4, &handlers32_overflow);
    IDT32_registerHandler(5, &handlers32_boundRangeExceed);
    IDT32_registerHandler(6, &handlers32_invaildOpcode);
    IDT32_registerHandler(7, &handlers32_deviceNotAvailable);
    IDT32_registerHandler(8, &handlers32_doubleFault);
    IDT32_registerHandler(9, &handlers32_coprocessorSegmentOverrun);
    IDT32_registerHandler(10, &handlers32_invalidTSS);
    IDT32_registerHandler(11, &handlers32_segmentNotPresent);
    IDT32_registerHandler(12, &handlers32_stackSegmentFault);
    IDT32_registerHandler(13, &handlers32_generalProtectionFault);
    IDT32_registerHandler(14, &handlers32_pageFault);
    IDT32_registerHandler(16, &handlers32_FPUx87Exception);
    IDT32_registerHandler(17, &handlers32_alignmentCheck);
    IDT32_registerHandler(18, &handlers32_machineCheck);
    IDT32_registerHandler(19, &handlers32_SIMDFloatingPoint);
    IDT32_registerHandler(20, &handlers32_virtualization);
    IDT32_registerHandler(30, &handlers32_security);

    helper_setEntry32(IRQ0, (uint32) &irq_32_0, 0x08, 0x8E);
    helper_setEntry32(IRQ1, (uint32) &irq_32_1, 0x08, 0x8E);
    helper_setEntry32(IRQ2, (uint32) &irq_32_2, 0x08, 0x8E);
    helper_setEntry32(IRQ3, (uint32) &irq_32_3, 0x08, 0x8E);
    helper_setEntry32(IRQ4, (uint32) &irq_32_4, 0x08, 0x8E);
    helper_setEntry32(IRQ5, (uint32) &irq_32_5, 0x08, 0x8E);
    helper_setEntry32(IRQ6, (uint32) &irq_32_6, 0x08, 0x8E);
    helper_setEntry32(IRQ7, (uint32) &irq_32_7, 0x08, 0x8E);
    helper_setEntry32(IRQ8, (uint32) &irq_32_8, 0x08, 0x8E);
    helper_setEntry32(IRQ9, (uint32) &irq_32_9, 0x08, 0x8E);
    helper_setEntry32(IRQ10, (uint32) &irq_32_10, 0x08, 0x8E);
    helper_setEntry32(IRQ11, (uint32) &irq_32_11, 0x08, 0x8E);
    helper_setEntry32(IRQ12, (uint32) &irq_32_12, 0x08, 0x8E);
    helper_setEntry32(IRQ13, (uint32) &irq_32_13, 0x08, 0x8E);
    helper_setEntry32(IRQ14, (uint32) &irq_32_14, 0x08, 0x8E);
    helper_setEntry32(IRQ15, (uint32) &irq_32_15, 0x08, 0x8E);

    helper_setEntry32( 0, (uint32) &isr_32_0 , 0x08, 0x8E);
    helper_setEntry32( 1, (uint32) &isr_32_1 , 0x08, 0x8E);
    helper_setEntry32( 2, (uint32) &isr_32_2 , 0x08, 0x8E);
    helper_setEntry32( 3, (uint32) &isr_32_3 , 0x08, 0x8E);
    helper_setEntry32( 4, (uint32) &isr_32_4 , 0x08, 0x8E);
    helper_setEntry32( 5, (uint32) &isr_32_5 , 0x08, 0x8E);
    helper_setEntry32( 6, (uint32) &isr_32_6 , 0x08, 0x8E);
    helper_setEntry32( 7, (uint32) &isr_32_7 , 0x08, 0x8E);
    helper_setEntry32( 8, (uint32) &isr_32_8 , 0x08, 0x8E);
    helper_setEntry32( 9, (uint32) &isr_32_9 , 0x08, 0x8E);
    helper_setEntry32(10, (uint32) &isr_32_10, 0x08, 0x8E);
    helper_setEntry32(11, (uint32) &isr_32_11, 0x08, 0x8E);
    helper_setEntry32(12, (uint32) &isr_32_12, 0x08, 0x8E);
    helper_setEntry32(13, (uint32) &isr_32_13, 0x08, 0x8E);
    helper_setEntry32(14, (uint32) &isr_32_14, 0x08, 0x8E);
    helper_setEntry32(15, (uint32) &isr_32_15, 0x08, 0x8E);
    helper_setEntry32(16, (uint32) &isr_32_16, 0x08, 0x8E);
    helper_setEntry32(17, (uint32) &isr_32_17, 0x08, 0x8E);
    helper_setEntry32(18, (uint32) &isr_32_18, 0x08, 0x8E);
    helper_setEntry32(19, (uint32) &isr_32_19, 0x08, 0x8E);
    helper_setEntry32(20, (uint32) &isr_32_20, 0x08, 0x8E);
    helper_setEntry32(21, (uint32) &isr_32_21, 0x08, 0x8E);
    helper_setEntry32(22, (uint32) &isr_32_22, 0x08, 0x8E);
    helper_setEntry32(23, (uint32) &isr_32_23, 0x08, 0x8E);
    helper_setEntry32(24, (uint32) &isr_32_24, 0x08, 0x8E);
    helper_setEntry32(25, (uint32) &isr_32_25, 0x08, 0x8E);
    helper_setEntry32(26, (uint32) &isr_32_26, 0x08, 0x8E);
    helper_setEntry32(27, (uint32) &isr_32_27, 0x08, 0x8E);
    helper_setEntry32(28, (uint32) &isr_32_28, 0x08, 0x8E);
    helper_setEntry32(29, (uint32) &isr_32_29, 0x08, 0x8E);
    helper_setEntry32(30, (uint32) &isr_32_30, 0x08, 0x8E);
    helper_setEntry32(31, (uint32) &isr_32_31, 0x08, 0x8E);
    helper_setEntry32(128, (uint32) &isr_32_128, 0x08, 0x8E);

    IDT32_Load((uint32) &g_IDTPointer32);

    return ERROR_SUCCESS;
}

size_t IDT32_registerHandler(uint32 a_index, ISR32_PFN a_handler)
{
    if (a_handler == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_index >= IDT_ENTRIES)
    {
        return ERROR_INVALID_PARAMETER;
    }

    g_intHandlers[a_index] = a_handler;

    return ERROR_SUCCESS;
}

void IDT32_isrHandler(IntCpuState32 *a_state)
{
    ISR32_PFN handler = g_intHandlers[a_state->intNo];
    handler(a_state);
}

void IDT32_irqHandler(IntCpuState32 *a_state)
{
    if (a_state->intNo >= 40)
    {
        // Reset signal to slave
        io_outb(0xA0, 0x20);
    }

    // Reset signal to master
    io_outb(0x20, 0x20);

    ISR32_PFN handler = g_intHandlers[a_state->intNo];
    handler(a_state);
}
