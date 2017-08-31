#include "util/kstdlib/include/kstdio.h"

void KLOG_FATAL(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    __KLOG("FATAL", format, args);
    va_end(args);
}

void KLOG_ERROR(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    __KLOG("ERROR", format, args);
    va_end(args);
}

void KLOG_WARNING(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    __KLOG("WARNING", format, args);
    va_end(args);
}

void KLOG_INFO(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    __KLOG("INFO", format, args);
    va_end(args);
}

void KLOG_TRACE(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    __KLOG("TRACE", format, args);
    va_end(args);
}

void __KLOG(const char *logging_level, const char *format, va_list args) 
{
    kprintf("[%s][%s][%s] ", __FILE__, __LINE__, logging_level);
    kprintf(format, args);
}
