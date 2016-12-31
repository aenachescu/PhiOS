#include "types.h"
#include "arch/x86/idt32.h"
#include "arch/x86/asm_io.h"
#include "arch/x86/handlers.h"
#include "errors.h"
#include "cpu.h"
#include "kstring.h"

IDT_Entry32 g_IDTEntries32[IDT_ENTRIES];
IDT_Pointer32 g_IDTPointer32;

size_t IDT_init32()
{
    g_IDTPointer32.limit = sizeof(IDT_Entry32) * 256 - 1;
    g_IDTPointer32.base = (uint32) &g_IDTEntries32;

    kmemset(&g_IDTEntries32, 0, sizeof(IDT_Entry32) * 256);

    for (int i = 0; i < 256; i++)
    {
        IDT_RegisterHandler32(i, &standardHandler32);
    }

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
    return ERROR_SUCCESS;
}
