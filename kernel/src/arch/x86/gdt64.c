#include "types.h"
#include "arch/x86/gdt64.h"
#include "arch/x86/gdt32.h"
#include "errors.h"

size_t GDT64_init()
{
    return ERROR_SUCCESS;
}
