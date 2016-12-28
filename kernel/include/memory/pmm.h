#ifndef PhiOS_PhysicalMemoryManager
#define PhiOS_PhysicalMemoryManager

#include "types.h"
#include "errors.h"

struct PhysicalMemoryAllocator
{
    size_t startAddress;
    size_t endAddress;
    size_t frameSize; // in bytes
    size_t positionLastAllocatedFrame;
    size_t freeFramesNumber;
    size_t *bitmap;
};

/*
 * @brief Creates an allocator.
 *
 * @param a_pma Pointer to allocator. There will be created the allocator.
 * @param a_frameSize The frame size in bytes.
 * @param a_startAddress The starting address of the physical memory area.
 * @param a_endAddress The ending address of the physical memory area.
 *
 * @return
 *  ERROR_NULL_POINTER - if a_pma is null.
 *  ERROR_INVALID_PARAMETER - if a_frameSize is 0. If a_startAddress is equal
 *      with a_endAddress or a_endAddress < a_startAddress. If a_startAddress
 *      or a_endAddress is not aligned at a_frameSize.
 *  ERROR_UNINITIALIZED - if placement address allocator is not initialized.
 */
size_t PMM_CreateAllocator(struct PhysicalMemoryAllocator *a_pma, size_t a_frameSize,
                           size_t a_startAddress, size_t a_endAddress);

/*
 * @brief Allocates physical memory.
 *
 * @param a_framesNumber The frames number to be allocated continuous.
 * @param a_physicalAddress In this parameter will be returned the physical
 *                          address.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends succesfully.
 *  ERROR_UNINITIALIZED - if the physical memory allocator is not initialized.
 *  ERROR_NO_FREE_MEMORY - if the free frames number is less than a_framesNumber.
 *  ERROR_NOT_FOUND - if there is not the required number of free frames
 *      continuous.
 *  ERROR_NULL_POINTER - if a_physicalAddress or a_pma is null
 *  ERROR_INVALID_PARAMETER - if a_framesNumber is 0.
 */
size_t PMM_Alloc(struct PhysicalMemoryAllocator *a_pma,
                 size_t a_framesNumber, size_t *a_physicalAddress);

/*
 * @brief Frees physical memory.
 *
 * @param a_framesNumber The frames number to be freed.
 * @param a_physicalAddress The starting address of memory area.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends successfully.
 *  ERROR_UNINITIALIZED - if the physical memory allocator is not initialized.
 *  ERROR_INVALID_PARAMETER - if a_framesNumber is 0 or a_physicalAddress is
 *      invalid (it is not aligned or it is bigger than RAM size).
 *  ERROR_INVALID_STATE - if at least one frame is not allocated (from the
 *      memory area to be freed).
 *  ERROR_NULL_POINTER - if a_pma is null.
 */
size_t PMM_Free(struct PhysicalMemoryAllocator *a_pma,
                size_t a_framesNumber, size_t a_physicalAddress);

#endif

