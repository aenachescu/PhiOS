#include "kernel/include/arch/x86/drng.h"

static uint64 g_random = 1;

uint64 drng_pseudoRandom()
{
    uint64 value = 0;

    uint64 time;
    asm volatile ("rdtsc" : "=A" (time));

    g_random = time * 310412 + 1101352L;
    value = g_random * 214013L + 2531011L;

    return value;
}

uint32 drng_uint16(uint16 *a_value)
{
    uint8 noError = 0;

    asm volatile ("rdrand %0; setc %1" :
                  "=r" (*a_value), "=qm" (noError));

    if (noError != 0) {
        return ERROR_SUCCESS;
    }

    return ERROR_DRNG_NOT_READY;
}

uint32 drng_uint32(uint32 *a_value)
{
    uint8 noError = 0;

    asm volatile ("rdrand %0; setc %1" :
                  "=r" (*a_value), "=qm" (noError));

    if (noError != 0) {
        return ERROR_SUCCESS;
    }

    return ERROR_DRNG_NOT_READY;
}

uint32 drng_uint64(uint64 *a_value)
{
    uint8 noError = 0;

    asm volatile ("rdrand %0; setc %1" :
                  "=r" (*a_value), "=qm" (noError));

    if (noError != 0) {
        return ERROR_SUCCESS;
    }

    return ERROR_DRNG_NOT_READY;
}
