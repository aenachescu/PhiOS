#include "types.h"
#include "arch/x86/drng.h"
#include "arch/x86/drng.h"
#include "errors.h"

size_t drng_uint16(uint16 *a_value)
{
    uint8 noError;

    asm volatile ("rdrand %0; setc %1" :
                  "=r" (*a_value), "=qm" (noError));

    if (noError)
        return ERROR_SUCCESS;

    return ERROR_DRNG_NOT_READY;
}

size_t drng_uint32(uint32 *a_value)
{
    uint8 noError;

    asm volatile ("rdrand %0; setc %1" :
                  "=r" (*a_value), "=qm" (noError));
    while(1) {}
    if (noError)
        return ERROR_SUCCESS;

    return ERROR_DRNG_NOT_READY;
}

#ifdef PhiOS_ARCH_x86_64

size_t drng_uint64(uint64 *a_value)
{
    uint8 noError;

    asm volatile ("rdrand %0; setc %1" :
                  "=r" (*a_value), "=qm" (noError));

    if (noError)
        return ERROR_SUCCESS;

    return ERROR_DRNG_NOT_READY;
}

#endif // PhiOS_ARCH_x86_64