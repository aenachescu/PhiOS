#ifndef PhiOS_registers
#define PhiOS_registers

#include "include/types.h"

uint32 readCR0();

void writeCR0(uint32 a_value);

uint32 readCR3();

void writeCR3(uint32 a_value);

#endif
