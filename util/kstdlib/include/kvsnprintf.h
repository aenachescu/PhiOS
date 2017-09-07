#ifndef PhiOS_kvsnprintf
#define PhiOS_kvsnprintf

#include "include/types.h"
#include "include/errors.h"

#include <stdarg.h>

uint32 kvsnprintf(
    char *a_buffer,
    uint32 *a_length,
    const char *a_format,
    va_list a_arg
);

#endif
