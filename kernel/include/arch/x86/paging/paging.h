#ifndef PhiOS_arch_x86_paging
#define PhiOS_arch_x86_paging

#include "types.h"
#include "errors.h"

#define PAGING_TYPE_IA32    1
#define PAGING_TYPE_PAE     2
#define PAGING_TYPE_IA32E   3

struct Paging
{
    uint8 pagingType;
    void *pagingStruct;
};

#endif
