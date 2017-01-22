#ifndef PhiOS_arch_x86_paging
#define PhiOS_arch_x86_paging

#include "types.h"
#include "errors.h"

#define PAGING_TYPE_NONE        0

#define PAGING_TYPE_IA32_4KB    1
#define PAGING_TYPE_IA32_4MB    2

#define PAGING_TYPE_PAE_2MB     3
#define PAGING_TYPE_PAE_4KB     4

#define PAGING_TYPE_IA32E_4KB   5
#define PAGING_TYPE_IA32E_2MB   6
#define PAGING_TYPE_IA32E_1GB   7

#define PAGING_FLAG_USER                0x10000000
#define PAGING_FLAG_KERNEL              0x20000000

#define PAGING_FLAG_ALLOC_AT_ADDRESS    0x00000001
#define PAGING_FLAG_SHARED_MEMORY       0x00000002

#define PAGING_FLAG_DESTROY             0x00000010

struct AllocFuncParam
{
    uint8       pagingType;
    void       *param;
};

struct FreeFuncParam
{
    uint8       pagingType;
    void       *param;
};

struct Paging;

typedef size_t (*VMA_ALLOC_PFN)(struct Paging *a_paging,
                                struct AllocFuncParam *a_request,
                                size_t *a_address);

typedef size_t (*VMA_FREE_PFN)(struct Paging *a_paging,
                               struct FreeFuncParam *a_request);

struct Paging
{
    uint8           pagingType;
    uint8           locked;
    void           *pagingStruct;
    VMA_ALLOC_PFN   allocFn;
    VMA_FREE_PFN    freeFn;
    size_t          freeMappedVirtualMemory;
    size_t          freeVirtualMemory;
    size_t          lastAllocatedPage;
};

#endif
