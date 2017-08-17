#ifndef PhiOS_kstdlib
#define PhiOS_kstdlib

#include "types.h"

/*
 * @brief Converts an integer value (signed/unsigned on 32/64 bits) to a null
 *        terminated string.
 *
 * @param a_value The value which to be converted.
 * @param a_buffer The string in which is stored the result.
 * @param a_length The string length.
 * @param a_base The numerical base used to represent the a_value. For the
 *               moment just the following numerical bases are supported:
 *               2, 10, 16.
 *
 * @return
 *  ERROR_SUCCESS - If the function ends successfully. If the function returns
 *      this error code then in a_length will be stored the string length.
 *
 *  ERROR_NULL_POINTER - If a_buffer or a_length is null.
 *
 *  ERROR_INSUFFICIENT_POINTER - If the string length is too small. If the
 *      function returns this error code then in a_length will be stored
 *      required length of string.
 *
 *  ERROR_UNSUPPORTED - If a_base is different of 2, 10, 16.
 */
uint32 kitoa(
    sint32 a_value, 
    char *a_buffer, 
    size_t *a_length, 
    size_t a_base
);

uint32 kutoa(
    uint32 a_value, 
    char *a_buffer, 
    size_t *a_length, 
    size_t a_base
);

uint32 ku64toa(
    uint64 a_value, 
    char *a_buffer, 
    size_t *a_length, 
    size_t a_base
);

uint32 ki64toa(
    sint64 a_value, 
    char *a_buffer, 
    size_t *a_length, 
    size_t a_base
);


uint32 krand(
    size_t *a_value
);

uint32 kranduint(
    size_t *a_value,
    size_t a_start,
    size_t a_end
);

uint32 ksrand(
    uint64 a_seed
);


#endif
