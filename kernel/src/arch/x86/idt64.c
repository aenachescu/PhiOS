#include "types.h"
#include "arch/x86/idt32.h"
#include "arch/x86/idt64.h"
#include "arch/x86/asm_io.h"
#include "arch/x86/handlers.h"
#include "errors.h"
#include "cpu.h"
#include "kstring.h"

static struct IDT64_Entry   g_IDTEntries64[IDT_ENTRIES];
static struct IDT64         g_IDTPointer64;

size_t IDT64_init()
{
    return ERROR_SUCCESS;
}

size_t IDT64_SetEntry(/* TODO: find sizes for args */)
{
    return ERROR_SUCCESS;
}

size_t IDT64_RegisterHandler(uint32 a_n, ISR64_PFN a_handler)
{
    return ERROR_SUCCESS;
}
