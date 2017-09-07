#include "kernel/include/logging.h"

#include <stdarg.h>

#include "util/kstdlib/include/kvsnprintf.h"

#define BUFFER_SIZE 512

static LOGGING_WRITE_PFN g_pfn[LOGGING_MAX_PFN];
static uint32 g_numOfPfn;
static bool g_isInitialized = false;

void __klog(
    const char *a_format,
    ...)
{
    char buffer[BUFFER_SIZE];
    uint32 length = BUFFER_SIZE;

    va_list arg;
    va_start(arg, a_format);

    uint32 err = kvsnprintf(buffer, &length, a_format, arg);

    va_end(arg);

    if (err == ERROR_SUCCESS || err == ERROR_INSUFFICIENT_BUFFER) {
        for (uint32 i = 0; i < g_numOfPfn; i++) {
            g_pfn[i](buffer);
        }
    }
}

uint32 logging_init()
{
    if (g_isInitialized == true) {
        return ERROR_ALREADY_INITIALIZED;
    }

    g_numOfPfn = 0;
    for (uint32 i = 0; i < LOGGING_MAX_PFN; i++) {
        g_pfn[i] = NULL;
    }

    g_isInitialized = true;

    return ERROR_SUCCESS;
}

uint32 logging_uinit()
{
    if (g_isInitialized == false) {
        return ERROR_UNINITIALIZED;
    }

    g_isInitialized = false;

    return ERROR_SUCCESS;
}

bool logging_isInitialized()
{
    return g_isInitialized;
}

uint32 logging_addPfn(
    LOGGING_WRITE_PFN a_pfn)
{
    if (g_isInitialized == false) {
        return ERROR_UNINITIALIZED;
    }

    if (g_numOfPfn == LOGGING_MAX_PFN) {
        return ERROR_LIMIT_REACHED;
    }

    if (a_pfn == NULL) {
        return ERROR_INVALID_FUNCTION;
    }

    g_pfn[g_numOfPfn] = a_pfn;
    g_numOfPfn++;

    return ERROR_SUCCESS;
}
