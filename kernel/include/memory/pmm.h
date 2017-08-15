#ifndef PhiOS_PhysicalMemoryManager
#define PhiOS_PhysicalMemoryManager

#include "types.h"
#include "errors.h"

#define PMM_FOR_VIRTUAL_MEMORY  1
#define PMM_FOR_DMA             2

typedef uint32 (*PMA_ALLOC_PFN)(
    void *a_pma,
    uint64 a_size,
    uint64 *a_physicalAddress
);

typedef uint32 (*PMA_FREE_PFN)(
    void *a_pma,
    uint64 a_size,
    uint64 a_physicalAddress
);

typedef uint32 (*PMA_RESERVE_PFN)(
    void *a_pma,
    uint64 a_size,
    uint64 physicalAddress
);

struct PMA
{
    uint8           type;
    uint8           locked;
    void           *PMAStruct;
    PMA_ALLOC_PFN   allocFn;
    PMA_FREE_PFN    freeFn;
    PMA_RESERVE_PFN reserveFn;
};

uint32 PMM_init(
    uint8 a_allocatorsNumber
);

uint32 PMM_addAllocator(
    void *a_allocator,
    uint8 a_flag,
    PMA_ALLOC_PFN a_allocFn,
    PMA_FREE_PFN a_freeFn,
    PMA_RESERVE_PFN a_reserveFn
);

uint32 PMM_alloc(
    uint64 *a_address,
    uint64 a_size,
    uint8 a_flag
);

uint32 PMM_free(
    uint64 a_address,
    uint64 a_size,
    uint8 a_flag
);

uint32 PMM_reserve(
    uint64 a_address,
    uint64 a_size,
    uint8 a_flag
);

#endif
