#ifndef PhiOS_pmm
#define PhiOS_pmm

#include "types.h"
#include "errors.h"

#define PMM_FOR_VIRTUAL_MEMORY  1
#define PMM_FOR_DMA             2

struct PMA
{
    uint8 type;
    void *PMAStruct;
};

size_t PMA_init(uint8 a_allocatorsNumber);

size_t PMA_addAllocator(void *a_allocator, uint8 a_flag);

size_t PMA_alloc(size_t *a_address, size_t a_frames, uint8 a_flag);

size_t PMA_free(size_t a_startAddress, size_t a_frames, uint8 a_flag);

#endif
