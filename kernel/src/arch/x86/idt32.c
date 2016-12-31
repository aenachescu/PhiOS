#include "types.h"
#include "arch/x86/idt32.h"
#include "arch/x86/asm_io.h"
#include "arch/x86/handlers.h"
#include "errors.h"
#include "cpu.h"
#include "kstring.h"

IDT_Entry32 g_IDTEntries32[IDT_ENTRIES];
IDT_Pointer32 g_IDTPointer32;
ISRfunc32 g_intHandlers[IDT_ENTRIES];

size_t IDT_init32()
{
    g_IDTPointer32.limit = sizeof(IDT_Entry32) * 256 - 1;
    g_IDTPointer32.base = (uint32) &g_IDTEntries32;

    kmemset(&g_IDTEntries32, 0, sizeof(IDT_Entry32) * 256);

    for (int i = 0; i < 256; i++)
    {
        IDT_RegisterHandler32(i, &standardHandler32);
    }

    // ADD HERE OTHER HANDLERS

    // Initialize PIC
    io_outb(0x20, 0x11);
    io_outb(0xA0, 0x11);
    io_outb(0x21, 0x20);
    io_outb(0xA1, 0x28);
    io_outb(0x21, 0x04);
    io_outb(0xA1, 0x02);
    io_outb(0x21, 0x01);
    io_outb(0xA1, 0x01);
    io_outb(0x21, 0x0);
    io_outb(0xA1, 0x0);

    IDT_SetEntry32(IRQ0, (uint32) &irq_32_0, 0x08, 0x8E);
    IDT_SetEntry32(IRQ1, (uint32) &irq_32_1, 0x08, 0x8E);
    IDT_SetEntry32(IRQ2, (uint32) &irq_32_2, 0x08, 0x8E);
    IDT_SetEntry32(IRQ3, (uint32) &irq_32_3, 0x08, 0x8E);
    IDT_SetEntry32(IRQ4, (uint32) &irq_32_4, 0x08, 0x8E);
    IDT_SetEntry32(IRQ5, (uint32) &irq_32_5, 0x08, 0x8E);
    IDT_SetEntry32(IRQ6, (uint32) &irq_32_6, 0x08, 0x8E);
    IDT_SetEntry32(IRQ7, (uint32) &irq_32_7, 0x08, 0x8E);
    IDT_SetEntry32(IRQ8, (uint32) &irq_32_8, 0x08, 0x8E);
    IDT_SetEntry32(IRQ9, (uint32) &irq_32_9, 0x08, 0x8E);
    IDT_SetEntry32(IRQ10, (uint32) &irq_32_10, 0x08, 0x8E);
    IDT_SetEntry32(IRQ11, (uint32) &irq_32_11, 0x08, 0x8E);
    IDT_SetEntry32(IRQ12, (uint32) &irq_32_12, 0x08, 0x8E);
    IDT_SetEntry32(IRQ13, (uint32) &irq_32_13, 0x08, 0x8E);
    IDT_SetEntry32(IRQ14, (uint32) &irq_32_14, 0x08, 0x8E);
    IDT_SetEntry32(IRQ15, (uint32) &irq_32_15, 0x08, 0x8E);

    IDT_SetEntry32( 0, (uint32) &isr_32_0 , 0x08, 0x8E);
    IDT_SetEntry32( 1, (uint32) &isr_32_1 , 0x08, 0x8E);
    IDT_SetEntry32( 2, (uint32) &isr_32_2 , 0x08, 0x8E);
    IDT_SetEntry32( 3, (uint32) &isr_32_3 , 0x08, 0x8E);
    IDT_SetEntry32( 4, (uint32) &isr_32_4 , 0x08, 0x8E);
    IDT_SetEntry32( 5, (uint32) &isr_32_5 , 0x08, 0x8E);
    IDT_SetEntry32( 6, (uint32) &isr_32_6 , 0x08, 0x8E);
    IDT_SetEntry32( 7, (uint32) &isr_32_7 , 0x08, 0x8E);
    IDT_SetEntry32( 8, (uint32) &isr_32_8 , 0x08, 0x8E);
    IDT_SetEntry32( 9, (uint32) &isr_32_9 , 0x08, 0x8E);
    IDT_SetEntry32(10, (uint32) &isr_32_10, 0x08, 0x8E);
    IDT_SetEntry32(11, (uint32) &isr_32_11, 0x08, 0x8E);
    IDT_SetEntry32(12, (uint32) &isr_32_12, 0x08, 0x8E);
    IDT_SetEntry32(13, (uint32) &isr_32_13, 0x08, 0x8E);
    IDT_SetEntry32(14, (uint32) &isr_32_14, 0x08, 0x8E);
    IDT_SetEntry32(15, (uint32) &isr_32_15, 0x08, 0x8E);
    IDT_SetEntry32(16, (uint32) &isr_32_16, 0x08, 0x8E);
    IDT_SetEntry32(17, (uint32) &isr_32_17, 0x08, 0x8E);
    IDT_SetEntry32(18, (uint32) &isr_32_18, 0x08, 0x8E);
    IDT_SetEntry32(19, (uint32) &isr_32_19, 0x08, 0x8E);
    IDT_SetEntry32(20, (uint32) &isr_32_20, 0x08, 0x8E);
    IDT_SetEntry32(21, (uint32) &isr_32_21, 0x08, 0x8E);
    IDT_SetEntry32(22, (uint32) &isr_32_22, 0x08, 0x8E);
    IDT_SetEntry32(23, (uint32) &isr_32_23, 0x08, 0x8E);
    IDT_SetEntry32(24, (uint32) &isr_32_24, 0x08, 0x8E);
    IDT_SetEntry32(25, (uint32) &isr_32_25, 0x08, 0x8E);
    IDT_SetEntry32(26, (uint32) &isr_32_26, 0x08, 0x8E);
    IDT_SetEntry32(27, (uint32) &isr_32_27, 0x08, 0x8E);
    IDT_SetEntry32(28, (uint32) &isr_32_28, 0x08, 0x8E);
    IDT_SetEntry32(29, (uint32) &isr_32_29, 0x08, 0x8E);
    IDT_SetEntry32(30, (uint32) &isr_32_30, 0x08, 0x8E);
    IDT_SetEntry32(31, (uint32) &isr_32_31, 0x08, 0x8E);
    IDT_SetEntry32(128, (uint32) &isr_32_128, 0x08, 0x8E);

    IDT_Load32((uint32) &g_IDTPointer32);

    return ERROR_SUCCESS;
}

size_t IDT_SetEntry32(uint32 a_n, uint32 a_base,
                    uint16 a_selector, uint8 a_flags)
{
    g_IDTEntries32[a_n].lowOffset   = a_base & 0xFFFF;
    g_IDTEntries32[a_n].highOffset  = (a_base >> 16) & 0xFFFF;
    g_IDTEntries32[a_n].selector    = a_selector;
    g_IDTEntries32[a_n].alwaysZero  = 0;
    g_IDTEntries32[a_n].type        = a_flags;

    return ERROR_SUCCESS;
}

size_t IDT_RegisterHandler32(uint32 a_n, ISRfunc32 a_handler)
{
    g_intHandlers[a_n] = a_handler;

    return ERROR_SUCCESS;
}

void IDT_isrHandler32(IntCpuState32 *a_state)
{
    if (g_intHandlers[a_state->intNo] == NULL)
    {
        standardHandler32(a_state);
    }

    ISRfunc32 handler = g_intHandlers[a_state->intNo];
    handler(a_state);
}

void IDT_irqHandler32(IntCpuState32 *a_state)
{
    if (a_state->intNo >= 40)
    {
        // Reset signal to slave
        io_outb(0xA0, 0x20);
    }

    // Reset signal to master
    io_outb(0x20, 0x20);

    if (g_intHandlers[a_state->intNo] == NULL)
    {
        standardHandler32(a_state);
    }

    ISRfunc32 handler = g_intHandlers[a_state->intNo];
    handler(a_state);
}
