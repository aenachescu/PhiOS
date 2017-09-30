#ifndef PhiOS_DRIVERS_CONFIG
#define PhiOS_DRIVERS_CONFIG

#ifdef STATIC_LINKAGE

#include "kernel/include/memory/paa.h"

#else

#error "Unsupported linkage mode"

#endif

#endif
