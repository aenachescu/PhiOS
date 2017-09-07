#ifndef PhiOS_ksnprintf_helper
#define PhiOS_ksnprintf_helper

#include "include/types.h"
#include "include/errors.h"

#include <stdarg.h>

/*
 * Things different from standard:
 * 1. 'p' specifier
 *      a. if the zero flag is present then the addres is padded with zeroes to left.
 *
 * 2. 's' specifier
 *      a. if the '#' flag is used then the string is truncated to width (if width is not equal to 0).
 *
 * Return:
 *  - the number of chars that would have been written if a_length had been sufficiently,
 *    counting the terminating null character.
 *
 * Notes:
 *  - only when returned value is less than a_length the string has been completely written.
 *  - the input is not checked, the function expect that pointers are usable.
 */
uint32 ksnprintf_helper(
    char *a_buffer,
    uint32 a_length,
    const char *a_format,
    va_list a_arg
);

#endif
