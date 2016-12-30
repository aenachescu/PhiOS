#include "types.h"
#include "arch/x86/idt.h"
#include "errors.h"
#include "cpu.h"

IDT_Entry32 IDTEntries32[IDT_ENTRIES];
IDT_Entry64 IDTEntries64[IDT_ENTRIES];
IDT_Pointer32 IDTPointer32;
IDT_Pointer64 IDTPointer64;

size_t IDT_init32()
{
    return ERROR_SUCCESS;
}

size_t IDT_init64()
{
    return ERROR_SUCCESS;
}

size_t IDT_SetEntry32(uint32 a_n, uint32 a_base,
                    uint16 a_selector, uint8 a_flags)
{
    return ERROR_SUCCESS;
}

size_t IDT_SetEntry64(/* TODO: find sizes for args */)
{
    return ERROR_SUCCESS;
}

size_t IDT_RegisterHandler32(uint32 a_n, ISRfunc32 a_handler)
{
    return ERROR_SUCCESS;
}

size_t IDT_RegisterHandler64(uint32 a_n, ISRfunc64 a_handler)
{
    return ERROR_SUCCESS;
}
