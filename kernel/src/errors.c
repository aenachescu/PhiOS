#include "errors.h"
#include "util/kstdlib/include/kstdio.h"
#include "cpu.h"
#include "types.h"

#define ERROR_KPRINTF(message) kprintf("\n[%s] In file %s at line %d.", message, file, line); stopCpu();

void __kernel_check(size_t err, const char *file, size_t line)
{
    switch (err) {
        case ERROR_SUCCESS:
            break;

        case ERROR_NULL_POINTER:
            ERROR_KPRINTF("ERROR_NULL_POINTER");
            break;

        case ERROR_INSUFFICIENT_BUFFER:
            ERROR_KPRINTF("ERROR_INSUFFICIENT_BUFFER");
            break;

        case ERROR_UNKNOWN:
            ERROR_KPRINTF("ERROR_UNKNOWN");
            break;

        case ERROR_UNSUPPORTED:
            ERROR_KPRINTF("ERROR_UNSUPPORTED");
            break;

        case ERROR_SAME_POINTERS:
            ERROR_KPRINTF("ERROR_SAME_POINTERS");
            break;

        case ERROR_EMPTY_BUFFER:
            ERROR_KPRINTF("ERROR_EMPTY_BUFFER");
            break;

        case ERROR_NOT_FOUND:
            ERROR_KPRINTF("ERROR_NOT_FOUND");
            break;

        case ERROR_UNINITIALIZED:
            ERROR_KPRINTF("ERROR_UNINITIALIZED");
            break;

        case ERROR_INVALID_PARAMETER:
            ERROR_KPRINTF("ERROR_INVALID_PARAMETER");
            break;

        case ERROR_ALREADY_INITIALIZED:
            ERROR_KPRINTF("ERROR_ALREADY_INITIALIZED");
            break;

        case ERROR_NO_FREE_MEMORY:
            ERROR_KPRINTF("ERROR_NO_FREE_MEMORY");
            break;

        case ERROR_INVALID_STATE:
            ERROR_KPRINTF("ERROR_INVALID_STATE");
            break;

        case ERROR_UNALIGNED_ADDRESS:
            ERROR_KPRINTF("ERROR_UNALIGNED_ADDRESS");
            break;

        case ERROR_UNKNOWN_FLAG:
            ERROR_KPRINTF("ERROR_UNKNOWN_FLAG");
            break;

        case ERROR_LIMIT_REACHED:
            ERROR_KPRINTF("ERROR_LIMIT_REACHED");
            break;

        case ERROR_INVALID_FUNCTION:
            ERROR_KPRINTF("ERROR_INVALID_FUNCTION");
            break;

        case ERROR_NOT_IMPLEMENTED:
            ERROR_KPRINTF("ERROR_NOT_IMPLEMENTED");
            break;

        case ERROR_DRNG_NOT_READY:
            ERROR_KPRINTF("ERROR_DRNG_NOT_READY");
            break;

        case ERROR_INVALID_PORT:
            ERROR_KPRINTF("ERROR_INVALID_PORT");

        case ERROR_PERMISSION_DENIED:
            ERROR_KPRINTF("ERROR_PERMISSION_DENIED");

        case ERROR_NO_FREE_VIRTUAL_MEMORY:
            ERROR_KPRINTF("ERROR_NO_FREE_VIRTUAL_MEMORY");

        default:
            ERROR_KPRINTF("UNKNOWN_ERROR_CODE");
            break;
    }
}
