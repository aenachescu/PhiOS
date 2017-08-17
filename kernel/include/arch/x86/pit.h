#ifndef PhiOS_ProgrammableIntervalTimer
#define PhiOS_ProgrammableIntervalTimer

#include "include/types.h"
#include "include/errors.h"

#define OSCILLATOR_FREQUENCY 1193182

uint32 PIT_init(
    uint16 a_divisor
);

#endif
