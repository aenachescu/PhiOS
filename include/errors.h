#ifndef PhiOS_errors
#define PhiOS_errors

#include "types.h"

#define ERROR_SUCCESS               0
#define ERROR_NULL_POINTER          1
#define ERROR_INSUFFICIENT_BUFFER   2
#define ERROR_UNKNOWN               3
#define ERROR_UNSUPPORTED           4
#define ERROR_SAME_POINTERS         5
#define ERROR_EMPTY_BUFFER          6
#define ERROR_NOT_FOUND             7
#define ERROR_UNINITIALIZED         8
#define ERROR_INVALID_PARAMETER     9
#define ERROR_ALREADY_INITIALIZED   10
#define ERROR_NO_FREE_MEMORY        11
#define ERROR_INVALID_STATE         12
#define ERROR_UNALIGNED_ADDRESS     13
#define ERROR_UNKNOWN_FLAG          14
#define ERROR_LIMIT_REACHED         15
#define ERROR_INVALID_FUNCTION      16
#define ERROR_NOT_IMPLEMENTED       17
#define ERROR_DRNG_NOT_READY        18

void __kernel_check(size_t err, const char *file, size_t line);
#define _kernel_check(err, file, line) __kernel_check(err, file, line)
#define KERNEL_CHECK(err) _kernel_check(err, __FILE__, __LINE__)

#endif
