#include "util/kstdlib/include/ksnprintf.h"
#include "util/kstdlib/include/ksnprintf_helper.h"

uint32 ksnprintf(
    char *a_buffer,
    uint32 *a_length,
    const char *a_format,
    ...)
{
    if (a_buffer == NULL ||
        a_length == NULL ||
        a_format == NULL) {
        return ERROR_NULL_POINTER;
    }

    va_list arg;
    va_start(arg, a_format);

    uint32 writtenBytes = ksnprintf_helper(a_buffer, *a_length, a_format, arg);

    va_end(arg);

    if (writtenBytes > *a_length) {
        a_buffer[*a_length - 1] = 0;
        *a_length = writtenBytes;
        return ERROR_INSUFFICIENT_BUFFER;
    }

    *a_length = writtenBytes;
    return ERROR_SUCCESS;
}
