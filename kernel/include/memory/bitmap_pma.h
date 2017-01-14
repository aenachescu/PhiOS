#ifndef PhiOS_BitmapPhysicalMemoryAllocator
#define PhiOS_BitmapPhysicalMemoryAllocator

#include "types.h"
#include "errors.h"

struct BitmapPMA
{
    size_t startAddress;
    size_t endAddress;
    size_t frameSize; // in bytes
    size_t positionLastAllocatedFrame;
    size_t freeFramesNumber;
    size_t bitmapSize;
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
 *  ERROR_INVALID_PARAMETER - If a_startAddress is equal with a_endAddress or
 *      a_endAddress < a_startAddress.
 *      If a_startAddress or a_endAddress is not aligned at a_frameSize.
 *      If a_frameSize is not a multiple of 2 or it's less than 2.
 *  ERROR_UNINITIALIZED - if placement address allocator is not initialized.
 */
size_t BitmapPMA_createAllocator(struct BitmapPMA *a_bpma,
                                 size_t a_frameSize,
                                 size_t a_startAddress,
                                 size_t a_endAddress);

/*
 * @brief Allocates physical memory.
 *
 * @param a_bpma Pointer to the bitmap physical memory allocator structure. In
 *               this structure will be allocated the memory.
 * @param a_framesNumber The frames number to be allocated continuous.
 * @param a_physicalAddress In this parameter will be returned the physical
 *                          address.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends succesfully.
 *  ERROR_UNINITIALIZED - if the physical memory allocator is not initialized.
 *      (a_pma->bitmap is null)
 *  ERROR_NO_FREE_MEMORY - if the free frames number is less than a_framesNumber.
 *  ERROR_NOT_FOUND - if there is not the required number of free frames
 *      continuous.
 *  ERROR_NULL_POINTER - if a_physicalAddress or a_pma is null
 *  ERROR_INVALID_PARAMETER - if a_framesNumber is 0.
 */
size_t BitmapPMA_alloc(void *a_bpma,
                       size_t a_framesNumber,
                       size_t *a_physicalAddress);

/*
 * @brief Frees physical memory.
 *
 * @param a_bpma Pointer to the bitmap physical memory allocator structure. In
 *               this structure will be freed the memory.
 * @param a_framesNumber The frames number to be freed.
 * @param a_physicalAddress The starting address of memory area.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends successfully.
 *  ERROR_UNINITIALIZED - if the physical memory allocator is not initialized.
 *  ERROR_INVALID_PARAMETER - if a_framesNumber is 0 or a_physicalAddress is
 *      not aligned.
 *      If a_physicalAddress is less than the starting address or is bigger
 *      than the ending address.
 *      If a_framesNumber is bigger than the frames number from the memory area.
 *  ERROR_INVALID_STATE - if at least one frame is not allocated (from the
 *      memory area to be freed).
 *      If a_framesNumber is bigger than the frames number between
 *      a_physicalAddress and the ending address.
 *  ERROR_NULL_POINTER - if a_pma is null.
 */
size_t BitmapPMA_free(void *a_bpma,
                      size_t a_framesNumber,
                      size_t a_physicalAddress);

#endif
