#include "paa.h"

extern size_t linker_kernelEnd;
static size_t g_placementAddress = 0;

#define ALIGN_PLACEMENT_ADDRESS                             \
    if (g_placementAddress & (WORDSIZE_BYTES - 1))          \
    {                                                       \
        g_placementAddress &= (~(WORDSIZE_BYTES - 1));      \
        g_placementAddress += WORDSIZE_BYTES;               \
    }

size_t PAA_init(void)
{
    if (g_placementAddress != 0)
    {
        return ERROR_ALREADY_INITIALIZED;
    }

    g_placementAddress = (size_t) &linker_kernelEnd;

    ALIGN_PLACEMENT_ADDRESS

    return ERROR_SUCCESS;
}

size_t PAA_alloc(size_t a_size, size_t *a_address)
{
    *a_address = 0;

    if (g_placementAddress == 0)
    {
        return ERROR_UNINITIALIZED;
    }

    if (a_size == 0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if (a_address == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    *a_address = g_placementAddress;

    g_placementAddress += a_size;
    ALIGN_PLACEMENT_ADDRESS

    return ERROR_SUCCESS;
}
