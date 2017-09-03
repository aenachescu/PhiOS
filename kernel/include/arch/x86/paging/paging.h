#ifndef PhiOS_arch_x86_paging
#define PhiOS_arch_x86_paging

#include "include/types.h"
#include "include/errors.h"

#define PAGING_TYPE_NONE        0

#define PAGING_TYPE_IA32_4KB    1
#define PAGING_TYPE_IA32_4MB    2

#define PAGING_TYPE_PAE_2MB     3
#define PAGING_TYPE_PAE_4KB     4

#define PAGING_TYPE_IA32E_4KB   5
#define PAGING_TYPE_IA32E_2MB   6
#define PAGING_TYPE_IA32E_1GB   7

#define PAGING_ALLOC_PAGE_FLAG_USER             0x00000001
#define PAGING_ALLOC_PAGE_FLAG_WRITE            0x00000002
#define PAGING_ALLOC_PAGE_FLAG_CACHE_DISABLED   0x00000004
#define PAGING_ALLOC_PAGE_FLAG_WRITE_THROUGH    0x00000008
#define PAGING_ALLOC_PAGE_FLAG_EXECUTE          0x00000010
#define PAGING_ALLOC_PAGE_FLAG_GLOBAL           0x00000020
#define PAGING_ALLOC_PAGE_FLAG_DEBUG            0x40000000
#define PAGING_ALLOC_PAGE_FLAG_UNIT_TESTING     0x80000000

#define PAGING_ALLOC_FLAG_AT_ADDRESS            0x00000001
#define PAGING_ALLOC_FLAG_SHARED_MEMORY         0x00000002
#define PAGING_ALLOC_FLAG_CLOSER_OF_ADDRESS     0x00000004
#define PAGING_ALLOC_FLAG_MAPS_KERNEL           0x00000008
#define PAGING_ALLOC_FLAG_DEBUG                 0x40000000
#define PAGING_ALLOC_FLAG_UNIT_TESTING          0x80000000

#define PAGING_FREE_FLAG_DESTROY                0x00000001
#define PAGING_FREE_FLAG_SHARED_MEMORY          0x00000002
#define PAGING_FREE_FLAG_EXIT_PROCESS           0x00000004
#define PAGING_FREE_FLAG_DEBUG                  0x40000000
#define PAGING_FREE_FLAG_UNIT_TESTING           0x80000000

struct VirtualAllocRequest
{
    uint32 flags;
    uint32 pageFlags;
    uint64 virtualAddress;
    uint64 length; // in bytes
    uint64 physicalAddress;
};

struct VirtualFreeRequest
{
    uint32 flags;
    uint64 startAddress;
    uint64 length; // in bytes
};

struct Paging;

typedef size_t (*VMA_ALLOC_PFN)(
    struct Paging *a_paging,
    struct VirtualAllocRequest *a_request,
    uint64 *a_address
);

typedef size_t (*VMA_FREE_PFN)(
    struct Paging *a_paging,
    struct VirtualFreeRequest *a_request
);

struct Paging
{
    uint8           pagingType;
    uint8           locked;
    void           *pagingStruct; // physical address
    VMA_ALLOC_PFN   allocFn;
    VMA_FREE_PFN    freeFn;
    uint64          freeVirtualMemory;    // amount of free memory in bytes
    uint64          lastAllocatedAddress; // virtual address of last allocated page
};

struct KernelArea
{
    size_t textStartAddr;
    size_t textEndAddr;

    size_t rodataStartAddr;
    size_t rodataEndAddr;

    size_t dataStartAddr;
    size_t dataEndAddr;

    size_t bssStartAddr;
    size_t bssEndAddr;

    size_t endPlacementAddr;
};

#define VIRTUAL_ADDRESS_STATE_UNKNOWN       0x00000000
#define VIRTUAL_ADDRESS_STATE_USED          0x00000001
#define VIRTUAL_ADDRESS_STATE_FREE          0x00000002
#define VIRTUAL_ADDRESS_STATE_RESERVED      0x00000004
#define VIRTUAL_ADDRESS_STATE_IN_SWAP       0x00000008
#define VIRTUAL_ADDRESS_STATE_PRIVATE       0x00000010
#define VIRTUAL_ADDRESS_STATE_SHARED        0x00000020

#define VIRTUAL_ADDRESS_FLAG_UNKNOWN        0x00000000
#define VIRTUAL_ADDRESS_FLAG_READ_ONLY      0x00000001
#define VIRTUAL_ADDRESS_FLAG_READ_WRITE     0x00000002
#define VIRTUAL_ADDRESS_FLAG_EXECUTE        0x00000004
#define VIRTUAL_ADDRESS_FLAG_USER_MODE      0x00000008
#define VIRTUAL_ADDRESS_FLAG_GLOBAL         0x00000010
#define VIRTUAL_ADDRESS_FLAG_NO_CACHE       0x00000020
#define VIRTUAL_ADDRESS_FLAG_ACCESSED       0x00000040
#define VIRTUAL_ADDRESS_FLAG_DIRTY          0x00000080
#define VIRTUAL_ADDRESS_FLAG_WRITE_THROUGH  0x00000100

struct VirtualAddressInfo
{
    void *baseAddress;
    uint32 pageSize; // in KiBs
    uint32 flags;
    uint32 state;
};

#endif
