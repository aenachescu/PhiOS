#ifndef PhiOS_UnitTesting_config
#define PhiOS_UnitTesting_config

#include "include/types.h"

#if defined WORDSIZE && WORDSIZE == 32
#define CUT_32BIT_VERSION
#elif defined WORDSIZE && WORDSIZE == 64
#define CUT_64BIT_VERSION
#else
#error "Unknown wordsize"
#endif

#include "tests/asserts.h"

#include <cut.h>

#endif
