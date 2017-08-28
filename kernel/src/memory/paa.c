#include "kernel/include/memory/paa.h"

static uint64 g_placementAddress = 0;

uint32 PAA_init(
    uint64 a_startAddress)
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

uint32 PAA_alloc(
    uint64 a_size,
    uint64 *a_address,
    uint64 a_alignment)
{
    if (g_placementAddress == 0) {
        return ERROR_UNINITIALIZED;
    }

    if (a_size == 0 || a_alignment == 0) {
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

uint64 PAA_getCurrentAddress()
{
    return g_placementAddress;
}
