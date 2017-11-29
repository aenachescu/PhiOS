#ifndef PhiOS_logging
#define PhiOS_logging

#include "include/types.h"
#include "include/errors.h"

void __klog(
    const char *a_format,
    ...
);

#define PhiOS_LOGGING_DEFAULT_FORMAT  "[%#10s][%5d][%7s]: "
#define PhiOS_LOGGING_NEW_LINE        "\n"

#define KLOG_RAW(format, ...)                                       \
    __klog(format, ##__VA_ARGS__)

#define KLOG(format, ...)                                           \
    __klog(format PhiOS_LOGGING_NEW_LINE, ##__VA_ARGS__)

#define KLOG_FATAL(format, ...)                                     \
    __klog(                                                         \
        PhiOS_LOGGING_DEFAULT_FORMAT format PhiOS_LOGGING_NEW_LINE, \
        __FILENAME__,                                               \
        __LINE__,                                                   \
        "FATAL",                                                    \
        ##__VA_ARGS__                                               \
    )

#define KLOG_ERROR(format, ...)                                     \
    __klog(                                                         \
        PhiOS_LOGGING_DEFAULT_FORMAT format PhiOS_LOGGING_NEW_LINE, \
        __FILENAME__,                                               \
        __LINE__,                                                   \
        "ERROR",                                                    \
        ##__VA_ARGS__                                               \
    )

#define KLOG_WARNING(format, ...)                                   \
    __klog(                                                         \
        PhiOS_LOGGING_DEFAULT_FORMAT format PhiOS_LOGGING_NEW_LINE, \
        __FILENAME__,                                               \
        __LINE__,                                                   \
        "WARNING",                                                  \
        ##__VA_ARGS__                                               \
    )

#define KLOG_INFO(format, ...)                                      \
    __klog(                                                         \
        PhiOS_LOGGING_DEFAULT_FORMAT format PhiOS_LOGGING_NEW_LINE, \
        __FILENAME__,                                               \
        __LINE__,                                                   \
        "INFO",                                                     \
        ##__VA_ARGS__                                               \
    )

#define KLOG_TRACE(format, ...)                                     \
    __klog(                                                         \
        PhiOS_LOGGING_DEFAULT_FORMAT format PhiOS_LOGGING_NEW_LINE, \
        __FILENAME__,                                               \
        __LINE__,                                                   \
        "TRACE",                                                    \
        ##__VA_ARGS__                                               \
    )

#define KLOG_DEBUG(format, ...)                                     \
    __klog(                                                         \
        PhiOS_LOGGING_DEFAULT_FORMAT format PhiOS_LOGGING_NEW_LINE, \
        __FILENAME__,                                               \
        __LINE__,                                                   \
        "DEBUG",                                                    \
        ##__VA_ARGS__                                               \
    )

typedef uint32 (*logging_writePfn)(const char *str);

uint32 logging_init();

uint32 logging_uninit();

bool logging_isInitialized();

uint32 logging_getPfnArrayCapacity();

uint32 logging_getPfnArrayLength();

uint32 logging_addPfn(
    logging_writePfn a_pfn
);

void logging_adjustPointers(
    size_t a_offset
);

#endif
