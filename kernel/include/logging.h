#ifndef PhiOS_logging
#define PhiOS_logging

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

#endif