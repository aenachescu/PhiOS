#include "include/types.h"
#include "include/errors.h"

#include <stdlib.h>

size_t PAA_alloc(size_t a_size, size_t *a_address, size_t a_alignment)
{
    if (a_size == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (a_address == NULL) {
        return ERROR_NULL_POINTER;
    }
    *a_address = 0;

    size_t g_placementAddress = (size_t) malloc(a_size + a_alignment - 1);
    if ((g_placementAddress & (a_alignment - 1)) != 0) {
        g_placementAddress &= (~(a_alignment - 1));
        g_placementAddress += a_alignment;
    }

    *a_address = g_placementAddress;
    g_placementAddress += a_size;

    return ERROR_SUCCESS;
}
