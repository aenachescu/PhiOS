#include "pmm.h"

size_t PMM_CreateAllocator(struct PhysicalMemoryAllocator *a_pma, size_t a_frameSize,
                           size_t a_startAddress, size_t a_endAddress)
{
    if (a_pma == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_frameSize == 0 || a_endAddress <= a_startAddress)
    {
        return ERROR_INVALID_PARAMETER;
    }

    a_pma->startAddress                 = a_startAddress;
    a_pma->endAddress                   = a_endAddress;
    a_pma->frameSize                    = a_frameSize;
    a_pma->freeFramesNumber             = (a_endAddress - a_startAddress) / a_frameSize;
    a_pma->positionLastAllocatedFrame   = 0;

    return ERROR_SUCCESS;
}

size_t PMM_Alloc(struct PhysicalMemoryAllocator *a_pma,
                 size_t a_framesNumber, size_t *a_physicalAddress)
{
    if (a_pma == NULL || a_physicalAddress == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_framesNumber == 0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    return ERROR_SUCCESS;
}

size_t PMM_Free(struct PhysicalMemoryAllocator *a_pma,
                size_t a_framesNumber, size_t a_physicalAddress)
{
    if (a_pma == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    size_t framesNumber = (a_pma->endAddress - a_pma->startAddress) /
                          pma->frameSize;

    if (a_framesNumber == 0 || a_framesNumber > framesNumber)
    {
        return ERROR_INVALID_PARAMETER;
    }

    return ERROR_SUCCESS;
}

