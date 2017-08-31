#ifndef Logging
#define Logging

#include "util/kstdlib/include/kstdio.h"
#include "util/kstdlib/include/kstdlib.h"

#include "drivers/video/include/vga/text_mode.h"

#include <stdarg.h>

uint32 __klog(const char *a_format, ...);

#define __KLOG(errot_type, ...) { \ 
    __klog(__FILENAME__);         \
    __klog(__LINE__)              \
    __klog(error_type);           \
    __klog(__VA_ARGS__);          \ 
};                                \

#define KLOG_ERROR(...) { __KLOG("ERROR", __VA_ARGS__) };
#define KLOG_WARNING(...) { __KLOG("WARNING", __VA_ARGS__) };
#define KLOG_INFO(...) { __KLOG("INFO", __VA_ARGS__) };
#define KLOG_FATAL(...) { __KLOG("FATAL", __VA_ARGS__) };

#endif