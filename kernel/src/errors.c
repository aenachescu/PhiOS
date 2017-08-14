#include "errors.h"
#include "util/kstdlib/include/kstdio.h"
#include "cpu.h"

void __kernel_check(size_t err, const char *file, size_t line)
{
    switch (err)
    {
        case ERROR_SUCCESS:
            break;

        case ERROR_NULL_POINTER:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_NULL_POINTER",
                file,
                line);
            stopCpu();
            break;
        
        case ERROR_INSUFFICIENT_BUFFER:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_INSUFFICIENT_BUFFER",
                file,
                line);
            stopCpu();
            break;

        case ERROR_UNKNOWN:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_UNKNOWN",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_UNSUPPORTED:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_UNSUPPORTED",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_SAME_POINTERS:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_SAME_POINTERS",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_EMPTY_BUFFER:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_EMPTY_BUFFER",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_NOT_FOUND:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_NOT_FOUND",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_UNINITIALIZED:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_UNINITIALIZED",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_INVALID_PARAMETER:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_INVALID_PARAMETER",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_ALREADY_INITIALIZED:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_ALREADY_INITIALIZED",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_NO_FREE_MEMORY:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_NO_FREE_MEMORY",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_INVALID_STATE:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_INVALID_STATE",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_UNALIGNED_ADDRESS:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_UNALIGNED_ADDRESS",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_UNKNOWN_FLAG:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_UNKNOWN_FLAG",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_LIMIT_REACHED:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_LIMIT_REACHED",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_INVALID_FUNCTION:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_INVALID_FUNCTION",
                file,
                line);
            stopCpu();
            break;
            
        case ERROR_NOT_IMPLEMENTED:
            kprintf("\n[%s] In file %s at line %d.",
                "ERROR_NOT_IMPLEMENTED",
                file,
                line);
            stopCpu();
            break;
        
        default:
            kprintf("\n[%s] In file %s at line %d.",
                "UNKNOWN_ERROR_CODE",
                file,
                line);
            stopCpu();
            break;
    }
}