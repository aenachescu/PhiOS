#include "kernel/include/memory/paa.h"

static size_t g_placementAddress = 0;

size_t PAA_init(size_t a_startAddress)
{
    if (g_placementAddress != 0) {
        return ERROR_ALREADY_INITIALIZED;
    }

    if (a_startAddress == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    g_placementAddress = a_startAddress;

    return ERROR_SUCCESS;
}

size_t PAA_alloc(size_t a_size, size_t *a_address, size_t a_alignment)
{
    if (g_placementAddress == 0) {
        return ERROR_UNINITIALIZED;
    }

    if (a_size == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (a_address == NULL) {
        return ERROR_NULL_POINTER;
    }
    *a_address = 0;

    if ((g_placementAddress & (a_alignment - 1)) != 0) {
        g_placementAddress &= (~(a_alignment - 1));
        g_placementAddress += a_alignment;
    }

    *a_address = g_placementAddress;
    g_placementAddress += a_size;

    return ERROR_SUCCESS;
}

size_t PAA_getCurrentAddress()
{
    return g_placementAddress;
}
