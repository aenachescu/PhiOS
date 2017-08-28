#include "kernel/include/memory/bitmap_pma.h"
#include "kernel/include/memory/paa.h"
#include "types.h"
#include "errors.h"

#define CHECK_ALIGN(addr, align) ((addr & (align - 1)) != 0)
#define ALIGN(addr, align) addr = addr & (~(align - 1))
#define IS_POWER_OF_2(x) (!(x & (x - 1)))

/*
 * This function doesn't checks if the input is correct because is a helper
 * function and it expects the input to be correct.
 */
static void helper_marksBits(
    struct BitmapPMA *a_bpma,
    uint32 indexBitmap,
    uint32 indexBits,
    uint32 requiredFrames)
{
    indexBits++;

    do {
        do {
            indexBits--;

            // set bit
            a_bpma->bitmap[indexBitmap] |= (((size_t) 1) << indexBits);

            // checks if all the bits was marked
            requiredFrames--;
            if (requiredFrames == 0) {
                return;
            }
        } while(indexBits > 0); // iterates through bits

        indexBits = WORDSIZE; // reset indexBits
        indexBitmap++;
    } while (true); // iterates through bitmap
}

/*
 * This function doesn't checks if the input is correct because is a helper
 * function and it expects the input to be correct.
 */
 static void helper_freesBits(
    struct BitmapPMA *a_bpma,
    uint32 indexBitmap,
    uint32 indexBits,
    uint32 requiredFrames)
{
    indexBits++;

    do {
        do {
            indexBits--;

            // frees bit
            a_bpma->bitmap[indexBitmap] &= (~(((size_t) 1) << indexBits));

            // checks if all the bits was marked
            requiredFrames--;
            if (requiredFrames == 0) {
                return;
            }
        } while(indexBits > 0); // iterates through bits

        indexBits = WORDSIZE; // reset indexBits
        indexBitmap++;
    } while (true); // iterates through bitmap
}

uint32 BitmapPMA_createAllocator(
    struct BitmapPMA *a_bpma,
    uint32 a_frameSize,
    uint64 a_startAddress,
    uint64 a_endAddress)
{
    if (a_bpma == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_endAddress <= a_startAddress ||
        IS_POWER_OF_2(a_frameSize) == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    // align start address
    if (CHECK_ALIGN(a_startAddress, a_frameSize)) {
        ALIGN(a_startAddress, a_frameSize);
        a_startAddress += a_frameSize;
    }

    // align end address
    if (CHECK_ALIGN(a_endAddress, a_frameSize)) {
        ALIGN(a_endAddress, a_frameSize);
    }

    a_bpma->startAddress                 = a_startAddress;
    a_bpma->endAddress                   = a_endAddress;
    a_bpma->frameSize                    = a_frameSize;
    a_bpma->freeFramesNumber             = (a_endAddress - a_startAddress) / a_frameSize;
    a_bpma->positionLastAllocatedFrame   = 0;
    a_bpma->bitmap                       = NULL;
    a_bpma->bitmapSize                   = 0;

    uint32 framesNumber     = (a_endAddress - a_startAddress) / a_frameSize;
    uint32 bitmapSize       = framesNumber / WORDSIZE;
    uint32 remainingBits    = framesNumber % WORDSIZE;

    if (remainingBits != 0) {
        bitmapSize++;
    }

    uint32 error = PAA_alloc(
        sizeof(size_t) * bitmapSize,
        (size_t*) &a_bpma->bitmap,
        WORDSIZE_BYTES
    );

    if (error != ERROR_SUCCESS) {
        a_bpma->bitmap       = NULL;
        a_bpma->bitmapSize   = 0;

        return error;
    }

    a_bpma->bitmapSize = bitmapSize;
    for (uint32 i = 0; i < bitmapSize; i++) {
        a_bpma->bitmap[i] = (size_t) 0;
    }

    // if framesNumber is not multiple of WORDSIZE, marks the bits from bitmap
    // that exceed framesNumber.
    if (remainingBits != 0) {
        helper_marksBits(
            a_bpma,
            bitmapSize - 1,
            WORDSIZE - 1 - remainingBits,
            WORDSIZE - remainingBits
        );
    }

    return ERROR_SUCCESS;
}

uint32 BitmapPMA_alloc(
    void *a_bpma,
    uint64 a_size,
    uint64 *a_physicalAddress)
{
    struct BitmapPMA *bpma = (struct BitmapPMA*) a_bpma;

    if (bpma == NULL || a_physicalAddress == NULL) {
        return ERROR_NULL_POINTER;
    }

    *a_physicalAddress = 0;

    if (bpma->bitmap == NULL) {
        return ERROR_UNINITIALIZED;
    }

    uint32 framesNumber = a_size / (uint64) bpma->frameSize + (a_size % (uint64) bpma->frameSize ? 1 : 0);

    if (framesNumber == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (framesNumber > bpma->freeFramesNumber) {
        return ERROR_NO_FREE_MEMORY;
    }

#define SET_PROCESSING          \
    if (processing == 0)        \
    {    \
        processing = 1;         \
        i_start = i;            \
        j_start = j;            \
    }

#define RESET_PROCESSING                    \
    if (processing == 1)                    \
    {    \
        processing = 0;                     \
        i_start = 0;                        \
        j_start = 0;                        \
        requiredFrames = framesNumber;    \
    }

    //search from bpma->positionLastAllocatedFrame to the last frame
    uint32 requiredFrames = framesNumber;
    uint32 i = bpma->positionLastAllocatedFrame;
    uint32 j = 0;
    uint32 limit = bpma->bitmapSize;
    uint32 i_start = 0;
    uint32 j_start = 0;
    uint8 processing = 0;
    uint8 keepSearching = 1;

Searching:
    for (; i < limit; i++) {
        // checks if the all bits are marked
        if (bpma->bitmap[i] == (~((size_t) 0))) {
            RESET_PROCESSING;
            continue;
        }

        j = WORDSIZE;
        do {
            j--;
            if ((bpma->bitmap[i] & (((size_t) 1) << j)) == 0) {
                SET_PROCESSING

                requiredFrames--;
                if (requiredFrames == 0) {
                    // marks bits
                    helper_marksBits(bpma, i_start, j_start, framesNumber);

                    // computes the physical address
                    uint32 frames = i_start * WORDSIZE;
                    frames += (WORDSIZE - 1 - j_start);
                    *a_physicalAddress = (uint64) frames * (uint64) bpma->frameSize + bpma->startAddress;

                    // set the last position
                    bpma->positionLastAllocatedFrame = i;

                    bpma->freeFramesNumber -= framesNumber;

                    return ERROR_SUCCESS;
                }
            }
            else {
                RESET_PROCESSING;
            }
        } while (j > 0);
    }

    //search from the first frame to bpma->positionLastAllocatedFrame
    if (keepSearching == 1) {
        keepSearching = 0;
        RESET_PROCESSING;
        i = 0;
        limit = bpma->positionLastAllocatedFrame;

        goto Searching;
    }

    return ERROR_NOT_FOUND;

#undef SET_PROCESSING
#undef RESET_PROCESSING
}

uint32 BitmapPMA_free(
    void *a_bpma,
    uint64 a_size,
    uint64 a_physicalAddress)
{
    struct BitmapPMA *bpma = (struct BitmapPMA*) a_bpma;

    if (bpma == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (bpma->bitmap == NULL) {
        return ERROR_UNINITIALIZED;
    }

    if (a_size == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    uint32 framesNumber = a_size / bpma->frameSize + (a_size % bpma->frameSize ? 1 : 0);
    uint32 totalFramesNumber = (bpma->endAddress - bpma->startAddress) / bpma->frameSize;

    if (framesNumber > totalFramesNumber
        || (a_physicalAddress & (bpma->frameSize - 1)) != 0
        || a_physicalAddress < bpma->startAddress
        || a_physicalAddress >= bpma->endAddress) {
        return ERROR_INVALID_PARAMETER;
    }

    totalFramesNumber = (bpma->endAddress - a_physicalAddress) / bpma->frameSize;
    if (totalFramesNumber < framesNumber) {
        return ERROR_INVALID_STATE;
    }

    // computes indexBitmap and indexBits
    a_physicalAddress -= bpma->startAddress;
    totalFramesNumber = a_physicalAddress / bpma->frameSize;
    uint32 startIndexBitmap = totalFramesNumber / WORDSIZE;
    uint32 startIndexBits   = WORDSIZE - 1 - totalFramesNumber % WORDSIZE;

    uint32 indexBitmap = startIndexBitmap;
    uint32 indexBits   = startIndexBits;

    // checks if the all framesNumber frames are marked
    totalFramesNumber = framesNumber;
    indexBits++;
    do {
        do {
            indexBits--;

            // checks if bit is 0
            if ((bpma->bitmap[indexBitmap] & (((size_t) 1) << indexBits)) == 0) {
                return ERROR_INVALID_STATE;
            }

            totalFramesNumber--;
            if (totalFramesNumber == 0) {
                goto FreesBits;
            }
        } while(indexBits != 0);

        indexBits = WORDSIZE;
        indexBitmap++;
    } while (true);

FreesBits:
    helper_freesBits(bpma, startIndexBitmap, startIndexBits, framesNumber);

    bpma->freeFramesNumber += framesNumber;

    return ERROR_SUCCESS;
}

uint32 BitmapPMA_reserve(
    void *a_bpma,
    uint64 a_size,
    uint64 a_physicalAddress)
{
    struct BitmapPMA *bpma = (struct BitmapPMA*) a_bpma;

    if (bpma == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (bpma->bitmap == NULL) {
        return ERROR_UNINITIALIZED;
    }

    if (a_size == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    uint64 startAddress = a_physicalAddress;
    ALIGN(startAddress, bpma->frameSize);

    uint64 endAddress = a_physicalAddress + a_size;
    if (CHECK_ALIGN(endAddress, bpma->frameSize)) {
        ALIGN(endAddress, bpma->frameSize);
        endAddress += bpma->frameSize;
    }

    if (startAddress < bpma->startAddress ||
        endAddress   > bpma->endAddress) {
        return ERROR_INVALID_PARAMETER;
    }

    startAddress -= bpma->startAddress;
    endAddress -= bpma->startAddress;

    uint32 frameNumber = startAddress / (uint64) bpma->frameSize;
    uint32 framesToReserve = endAddress / (uint64) bpma->frameSize - frameNumber;

    if (framesToReserve > bpma->freeFramesNumber) {
        return ERROR_NO_FREE_MEMORY;
    }

    bpma->freeFramesNumber -= framesToReserve;

    uint32 bitmapIndex = frameNumber / WORDSIZE;
    uint32 indexBits = WORDSIZE - 1 - frameNumber % WORDSIZE;

    helper_marksBits(bpma, bitmapIndex, indexBits, framesToReserve);

    return ERROR_SUCCESS;
}

uint32 BitmapPMA_check(
    void *a_bpma,
    uint64 a_startAddr,
    uint64 a_endAddr,
    uint8 *a_state)
{
    struct BitmapPMA *bpma = (struct BitmapPMA*) a_bpma;

    if (bpma == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (bpma->bitmap == NULL) {
        return ERROR_UNINITIALIZED;
    }

    ALIGN(a_startAddr, bpma->frameSize);

    if (CHECK_ALIGN(a_endAddr, bpma->frameSize)) {
        ALIGN(a_endAddr, bpma->frameSize);
        a_endAddr += bpma->frameSize;
    }

    if (a_startAddr >= a_endAddr ||
        a_startAddr < bpma->startAddress ||
        a_endAddr > bpma->endAddress) {
        return ERROR_INVALID_PARAMETER;
    }

    if (*a_state != false && *a_state != true) {
        return ERROR_INVALID_PARAMETER;
    }

    a_startAddr -= bpma->startAddress;
    a_endAddr -= bpma->startAddress;
    uint32 frameNumber = a_startAddr / bpma->frameSize;
    uint32 framesToCheck = a_endAddr / bpma->frameSize - frameNumber;
    
    uint32 bitmapIndex = frameNumber / WORDSIZE;
    uint32 indexBits = WORDSIZE - 1 - frameNumber % WORDSIZE;
    
    uint8 state = *a_state;
    if (state) {
        *a_state = false;
    } else {
        *a_state = true;
    }

    indexBits++;
    do {
        do {
            indexBits--;

            if (state && (bpma->bitmap[bitmapIndex] & (((size_t) 1) << indexBits))) {
                *a_state = true;
                return ERROR_SUCCESS;
            } else if ((bpma->bitmap[bitmapIndex] & (((size_t) 1) << indexBits)) != 0) {
                *a_state = false;
                return ERROR_SUCCESS;
            }

            framesToCheck--;
            if (framesToCheck == 0) {
                return ERROR_SUCCESS;
            }
        } while (indexBits != 0);

        indexBits = WORDSIZE;
        bitmapIndex++;
    } while (true);

    return ERROR_SUCCESS;
}