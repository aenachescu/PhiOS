#include "include/types.h"
#include "include/errors.h"

#include <stdlib.h>

uint32 PAA_alloc(uint64 a_size, uint64 *a_address, uint64 a_alignment)
{
    if (a_size == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (a_address == NULL) {
        return ERROR_NULL_POINTER;
    }
    *a_address = 0;

    uint64 g_placementAddress = (uint64) (size_t) malloc(a_size + a_alignment - 1);
    if ((g_placementAddress & (a_alignment - 1)) != 0) {
        g_placementAddress &= (~(a_alignment - 1));
        g_placementAddress += a_alignment;
    }

    *a_address = g_placementAddress;
    g_placementAddress += a_size;

    return ERROR_SUCCESS;
}
