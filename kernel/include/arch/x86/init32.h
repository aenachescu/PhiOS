#ifndef PhiOS_arch_x86_init32
#define PhiOS_arch_x86_init32

#include "types.h"
#include "errors.h"

size_t init_init32(
    uint32 mboot2Magic, 
    uint32 mboot2Addr
);

#endif
