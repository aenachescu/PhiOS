#ifndef PhiOS_logging
#define PhiOS_logging

#include "include/types.h"
#include "include/errors.h"

void __klog(
    const char *a_file,
    int a_line,
    const char *a_logType,
    const char *a_format,
    ...
);

#define __KLOG(logType, format, ...) __klog(__FILENAME__, __LINE__, logType, format, ##__VA_ARGS__)

#define KLOG_FATAL(format, ...)     __KLOG("FATAL", format, ##__VA_ARGS__)
#define KLOG_ERROR(format, ...)     __KLOG("ERROR", format, ##__VA_ARGS__)
#define KLOG_WARNING(format, ...)   __KLOG("WARNING", format, ##__VA_ARGS__)
#define KLOG_INFO(format, ...)      __KLOG("INFO", format, ##__VA_ARGS__)
#define KLOG_TRACE(format, ...)     __KLOG("TRACE", format, ##__VA_ARGS__)

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