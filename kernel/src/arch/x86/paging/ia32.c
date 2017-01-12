#include "arch/x86/paging/ia32.h"
#include "memory/pmm.h"

size_t IA32_4KB_initKernelStruct(struct Paging *a_paging,
                                 size_t a_codeStartAddr,
                                 size_t a_codeEndAddr,
                                 size_t a_rodataStartAddr,
                                 size_t a_rodataEndAddr,
                                 size_t a_rwdataStartAddr,
                                 size_t a_rwdataEndAddr)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_codeEndAddr   <= a_codeStartAddr    ||
        a_rodataEndAddr <  a_rodataStartAddr  ||
        a_rwdataEndAddr <  a_rwdataStartAddr)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if ((a_codeStartAddr   & 4095) != 0 ||
        (a_rodataStartAddr & 4095) != 0 ||
        (a_rwdataStartAddr & 4095) != 0)
    {
        return ERROR_UNALIGNED_ADDRESS;
    }

    size_t error = ERROR_SUCCESS;

    do
    {
    } while (false);

    return error;
}
