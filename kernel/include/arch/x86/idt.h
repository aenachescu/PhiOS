//
// Created by littlewho on 7/3/17.
//

#ifndef PhiOS_idt
#define PhiOS_idt

#ifdef PhiOS_ARCH_x86_32
#include "arch/x86/idt32.h"
#define IDT_registerHandler IDT32_registerHandler
#endif

#ifdef PhiOS_ARCH_x86_64
#include "arch/x86/idt64.h"
#define IDT_registerHandler IDT64_registerHandler
#endif

#endif //PHIOS_IDT_H
