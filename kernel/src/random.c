#include "types.h"
#include "random.h"
#include "errors.h"

#if defined PhiOS_ARCH_x86_32 || defined PhiOS_ARCH_x86_64

#include "kernel/include/arch/x86/drng.h"
#include "kernel/include/arch/x86/cpuid.h"

#endif

static uint64 g_random;

static size_t helper_hardwareSeed()
{
    size_t value;

    uint64 time;
    asm volatile ("rdtsc" : "=A" (time));

    g_random = (((time * 310412 + 1101352L) >> 16) & 0x7fff);
    value = (((g_random * 214013L + 2531011L) >> 16) & 0x7fff);

    return value;
}

uint16 kernel_random16()
{
    uint16 value;

#if defined PhiOS_ARCH_x86_32 || defined PhiOS_ARCH_x86_64
    if (CPUID_HasFeature(CPU_FEATURE_RDRAND) == ERROR_SUCCESS) while (drng_uint16(&value) == ERROR_DRNG_NOT_READY) {}
    else
    {
        value = (uint16) helper_hardwareSeed();
    }
#endif

    return value;
}

uint32 kernel_random32()
{
    uint32 value;

#if defined PhiOS_ARCH_x86_32 || defined PhiOS_ARCH_x86_64
    if (CPUID_HasFeature(CPU_FEATURE_RDRAND) == ERROR_SUCCESS) while (drng_uint32(&value) == ERROR_DRNG_NOT_READY) {}
    else
    {
        value = (uint32) helper_hardwareSeed();
    }
#endif

    return value;
}

uint64 kernel_random64()
{
    uint64 value;

#if defined PhiOS_ARCH_x86_64
    if (CPUID_HasFeature(CPU_FEATURE_RDRAND) == ERROR_SUCCESS) while (drng_uint64(&value) == ERROR_DRNG_NOT_READY) {}
    else
    {
        value = (uint64) helper_hardwareSeed();
    }
#endif // PhiOS_ARCH_x86_64

    return value;
}

size_t kernel_random()
{
    size_t value;

#ifdef PhiOS_ARCH_x86_32
    if (CPUID_HasFeature(CPU_FEATURE_RDRAND) == ERROR_SUCCESS) while (drng_uint32(&value) == ERROR_DRNG_NOT_READY) {}
    else
    {
        value = (uint32) helper_hardwareSeed();
    }
#elif PhiOS_ARCH_x86_64
    if (CPUID_HasFeature(CPU_FEATURE_RDRAND) == ERROR_SUCCESS) while (drng_uint64(&value) == ERROR_DRNG_NOT_READY) {}
    else
    {
        value = (uint64) helper_hardwareSeed();
    }
#endif

    return value;
}