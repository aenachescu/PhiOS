#ifndef PhiOS_PhysicalMemoryManager
#define PhiOS_PhysicalMemoryManager

#include "types.h"
#include "errors.h"

#define PMM_FOR_VIRTUAL_MEMORY  1
#define PMM_FOR_DMA             2

typedef size_t (*PMA_ALLOC_PFN)(void   *a_pma,
                                size_t  a_framesNumber,
                                size_t *a_physicalAddress);

typedef size_t (*PMA_FREE_PFN)(void  *a_pma,
                               size_t a_framesNumber,
                               size_t a_physicalAddress);

struct PMA
{
    uint8           type;
    uint8           locked;
    void           *PMAStruct;
    PMA_ALLOC_PFN   allocFn;
    PMA_FREE_PFN    freeFn;
};

size_t PMM_init(uint8 a_allocatorsNumber);

size_t PMM_addAllocator(void *a_allocator, uint8 a_flag,
                        PMA_ALLOC_PFN a_allocFn, PMA_FREE_PFN a_freeFn);

size_t PMM_alloc(size_t *a_address, size_t a_frames, uint8 a_flag);

size_t PMM_free(size_t a_address, size_t a_frames, uint8 a_flag);

#endif
