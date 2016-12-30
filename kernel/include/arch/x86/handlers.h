#ifndef PhiOS_handlers
#define PhiOS_handlers

#include "arch/x86/idt.h"
#include "cpu.h"

void standardHandler32(IntCpuState32 *a_state);
void standardHandler64(IntCpuState64 *a_state);

#endif
