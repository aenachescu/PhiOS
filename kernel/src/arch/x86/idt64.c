#include "types.h"
#include "arch/x86/idt32.h"
#include "arch/x86/idt64.h"
#include "arch/x86/asm_io.h"
#include "arch/x86/handlers.h"
#include "errors.h"
#include "cpu.h"
#include "kstring.h"

IDT_Entry64 g_IDTEntries64[IDT_ENTRIES];
IDT_Pointer64 g_IDTPointer64;

size_t IDT_init64()
{
    return ERROR_SUCCESS;
}

size_t IDT_SetEntry64(/* TODO: find sizes for args */)
{
    return ERROR_SUCCESS;
}

size_t IDT_RegisterHandler64(uint32 a_n, ISRfunc64 a_handler)
{
    return ERROR_SUCCESS;
}
