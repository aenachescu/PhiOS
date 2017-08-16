#ifndef PhiOS_DigitalRandomNumberGenerator
#define PhiOS_DigitalRandomNumberGenerator

#include "types.h"

size_t drng_uint16(uint16 *a_value);

size_t drng_uint32(uint32 *a_value);

#ifdef PhiOS_ARCH_x86_64

size_t drng_uint64(uint64 *a_value);

#endif // PhiOS_ARCH_x86_64

#endif // PhiOS_DigitalRandomNumberGenerator