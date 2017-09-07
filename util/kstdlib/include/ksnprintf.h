#ifndef PhiOS_ksnprintf
#define PhiOS_ksnprintf

#include "include/types.h"
#include "include/errors.h"

uint32 ksnprintf(
    char *a_buffer,
    uint32 *a_length,
    const char *a_format,
    ...
);

#endif
