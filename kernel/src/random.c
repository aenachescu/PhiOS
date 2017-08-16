#include "random.h"
#include "errors.h"

#if defined PhiOS_ARCH_x86_32 || defined PhiOS_ARCH_x86_64

#include "kernel/include/arch/x86/drng.h"
#include "kernel/include/arch/x86/cpuid.h"

#endif

uint16 kernel_random16()
{
    uint16 value = 0;

#if defined PhiOS_ARCH_x86_32 || defined PhiOS_ARCH_x86_64
    if (CPUID_HasFeature(CPU_FEATURE_RDRAND) == ERROR_SUCCESS) {
        while (drng_uint16(&value) == ERROR_DRNG_NOT_READY) {}
    } else {
        value = (uint16) drng_pseudoRandom();
    }
#endif

    return value;
}

uint32 kernel_random32()
{
    uint32 value = 0;

#if defined PhiOS_ARCH_x86_32 || defined PhiOS_ARCH_x86_64
    if (CPUID_HasFeature(CPU_FEATURE_RDRAND) == ERROR_SUCCESS) {
        while (drng_uint32(&value) == ERROR_DRNG_NOT_READY) {}
    } else {
        value = (uint32) drng_pseudoRandom();
    }
#endif

    return value;
}

uint64 kernel_random64()
{
    uint64 value = 0;

#if defined PhiOS_ARCH_x86_32 || defined PhiOS_ARCH_x86_64
    if (CPUID_HasFeature(CPU_FEATURE_RDRAND) == ERROR_SUCCESS) {
        while (drng_uint64(&value) == ERROR_DRNG_NOT_READY) {}
    } else {
        value = (uint64) drng_pseudoRandom();
    }
#endif // PhiOS_ARCH_x86_64

    return value;
}
