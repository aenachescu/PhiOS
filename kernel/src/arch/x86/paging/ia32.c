#include "arch/x86/paging/ia32.h"

size_t IA32_4KB_initKernelStruct(struct Paging *a_paging,
                                 size_t a_kernelStartAddr,
                                 size_t a_kernelEndAddr)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_kernelEndAddr <= a_kernelStartAddr)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if ((a_kernelStartAddr & 4095) != 0)
    {
        return ERROR_UNALIGNED_ADDRESS;
    }

    return ERROR_SUCCESS;
}
