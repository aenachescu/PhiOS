#ifndef PhiOS_pmm
#define PhiOS_pmm

#include "types.h"
#include "errors.h"

#define PMM_BITMAP_PMA 1

struct PMA
{
    uint8 type;
    void *PMAStruct;
};

struct PMA *g_allocators;

size_t PMA_init(size_t a_allocatorsNumber);
size_t PMA_createAllocator(struct PMA *a_allocator, size_t a_startAddress,
                            size_t a_endAddress, uint8 a_flag);
size_t PMA_alloc(size_t *a_address, size_t a_frames, uint8 a_flag);
size_t PMA_free(size_t a_startAddress, size_t a_frames, uint8 a_flag);

#endif
