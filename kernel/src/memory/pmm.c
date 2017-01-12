#include "pmm.h"

struct PMA *g_allocators;
uint8 g_index;
uint8 g_allocatorsNumber;

size_t PMM_init(uint8 a_allocatorsNumber)
{
}

size_t PMM_addAllocator(void *a_pma, uint8 a_flag)
{
}

size_t PMA_alloc(size_t *a_address, size_t a_frames, uint8 a_flag)
{
}

size_t PMA_free(size_t a_startAddress, size_t a_frames, uint8 a_flag)
{
}
