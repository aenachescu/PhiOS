#ifndef PhiOS_ProgrammableIntervalTimer
#define PhiOS_ProgrammableIntervalTimer

#include "types.h"
#include "errors.h"

#define OSCILLATOR_FREQUENCY 1193182

size_t PIT_init(
    uint16 a_divisor
);

#endif
