#ifndef PhiOS_logging
#define PhiOS_logging

#include "include/types.h"
#include "include/errors.h"

void __klog(
    const char *a_format,
    ...
);

#define DEFAULT_FORMAT  "[%#10s][%5d][%7s]: "
#define NEW_LINE        "\n"

#define KLOG_FATAL(format, ...)         \
    __klog(                             \
        DEFAULT_FORMAT format NEW_LINE, \
        __FILENAME__,                   \
        __LINE__,                       \
        "FATAL",                        \
        ##__VA_ARGS__                   \
    )

#define KLOG_ERROR(format, ...)         \
    __klog(                             \
        DEFAULT_FORMAT format NEW_LINE, \
        __FILENAME__,                   \
        __LINE__,                       \
        "ERROR",                        \
        ##__VA_ARGS__                   \
    )

#define KLOG_WARNING(format, ...)       \
    __klog(                             \
        DEFAULT_FORMAT format NEW_LINE, \
        __FILENAME__,                   \
        __LINE__,                       \
        "WARNING",                      \
        ##__VA_ARGS__                   \
    )

#define KLOG_INFO(format, ...)          \
    __klog(                             \
        DEFAULT_FORMAT format NEW_LINE, \
        __FILENAME__,                   \
        __LINE__,                       \
        "INFO",                         \
        ##__VA_ARGS__                   \
    )

#define KLOG_TRACE(format, ...)         \
    __klog(                             \
        DEFAULT_FORMAT format NEW_LINE, \
        __FILENAME__,                   \
        __LINE__,                       \
        "TRACE",                        \
        ##__VA_ARGS__                   \
    )

typedef uint32 (*LOGGING_WRITE_PFN)(const char *str);

#ifndef LOGGING_MAX_PFN
#define LOGGING_MAX_PFN 5
#endif

uint32 logging_init();
uint32 logging_uninit();
bool logging_isInitialized();

uint32 logging_addPfn(
    LOGGING_WRITE_PFN a_pfn
);

#endif
