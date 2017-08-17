#include "kernel/include/arch/x86/init64.h"
#include "kernel/include/arch/x86/paging/paging.h"

struct KernelArea g_kernelArea;

uint32 init_init64(
    uint32 mboot2Magic, 
    uint32 mboot2Addr)
{
    return ERROR_NOT_IMPLEMENTED;
}