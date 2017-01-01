#ifndef PhiOS_handlers32
#define PhiOS_handlers32

#include "arch/x86/idt32.h"
#include "cpu.h"

void handlers32_default(IntCpuState32 *a_state);

#endif
