#ifndef PhiOS_arch_x86_init64
#define PhiOS_arch_x86_init64

#include "types.h"
#include "errors.h"

uint32 init_init64(
    uint32 mboot2Magic, 
    uint32 mboot2Addr
);

#endif
