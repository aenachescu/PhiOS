#ifndef PhiOS_DigitalRandomNumberGenerator
#define PhiOS_DigitalRandomNumberGenerator

#include "include/types.h"
#include "include/errors.h"

uint64 drng_pseudoRandom();

uint32 drng_uint16(
    uint16 *a_value
);

uint32 drng_uint32(
    uint32 *a_value
);

uint32 drng_uint64(
    uint64 *a_value
);

#endif // PhiOS_DigitalRandomNumberGenerator
