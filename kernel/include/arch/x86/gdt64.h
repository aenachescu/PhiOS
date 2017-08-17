#ifndef PhiOS_GlobalDescriptorTable64
#define PhiOS_GlobalDescriptorTable64

#include "include/types.h"
#include "include/errors.h"

uint32 GDT64_init();

extern void GDT_Load64(
    uint64 a_table
);

#endif
