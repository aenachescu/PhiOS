#include "util/kstdlib/include/kvsnprintf.h"
#include "util/kstdlib/include/ksnprintf_helper.h"

uint32 kvsnprintf(
    char *a_buffer,
    uint32 *a_length,
    const char *a_format,
    va_list a_arg)
{
    if (a_buffer == NULL ||
        a_length == NULL ||
        a_format == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 writtenBytes = ksnprintf_helper(a_buffer, *a_length, a_format, a_arg);

    if (writtenBytes > *a_length) {
        *a_length = writtenBytes;
        return ERROR_INSUFFICIENT_BUFFER;
    }

    *a_length = writtenBytes;
    return ERROR_SUCCESS;
}
