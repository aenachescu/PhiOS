#include "pmm.h"
#include "paa.h"


/*
 * This function doesn't checks if the input is correct because is a helper
 * function and it expects the input to be correct.
 */
static void helper_MarksBits(struct PhysicalMemoryAllocator *a_pma,
                             size_t indexBitmap, size_t indexBits,
                             size_t requiredFrames)
{
    indexBits++;

    do
    {
        do
        {
            indexBits--;

            // set bit
            a_pma->bitmap[indexBitmap] |= (((size_t) 1) << indexBits);

            // checks if all the bits was marked
            requiredFrames--;
            if (requiredFrames == 0)
            {
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
 static void helper_FreesBits(struct PhysicalMemoryAllocator *a_pma,
                              size_t indexBitmap, size_t indexBits,
                              size_t requiredFrames)
{
    indexBits++;

    do
    {
        do
        {
            indexBits--;

            // frees bit
            a_pma->bitmap[indexBitmap] &= (~(((size_t) 1) << indexBits));

            // checks if all the bits was marked
            requiredFrames--;
            if (requiredFrames == 0)
            {
                return;
            }
        } while(indexBits > 0); // iterates through bits

        indexBits = WORDSIZE; // reset indexBits
        indexBitmap++;
    } while (true); // iterates through bitmap
}

size_t PMM_CreateAllocator(struct PhysicalMemoryAllocator *a_pma, size_t a_frameSize,
                           size_t a_startAddress, size_t a_endAddress)
{
    if (a_pma == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_frameSize < 2 || a_endAddress <= a_startAddress ||
        ((a_frameSize - 1) & a_frameSize) != 0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if ((a_startAddress & (a_frameSize - 1)) != 0 ||
        (a_endAddress & (a_frameSize - 1)) != 0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    a_pma->startAddress                 = a_startAddress;
    a_pma->endAddress                   = a_endAddress;
    a_pma->frameSize                    = a_frameSize;
    a_pma->freeFramesNumber             = (a_endAddress - a_startAddress) / a_frameSize;
    a_pma->positionLastAllocatedFrame   = 0;
    a_pma->bitmap                       = NULL;
    a_pma->bitmapSize                   = 0;

    size_t framesNumber     = (a_endAddress - a_startAddress) / a_frameSize;
    size_t bitmapSize       = framesNumber / WORDSIZE;
    size_t remainingBits    = framesNumber % WORDSIZE;

    if (remainingBits != 0)
    {
        bitmapSize++;
    }

    size_t error = PAA_Alloc(sizeof(size_t) * bitmapSize, (size_t*) &a_pma->bitmap);
    if (error != ERROR_SUCCESS)
    {
        a_pma->bitmap       = NULL;
        a_pma->bitmapSize   = 0;

        return error;
    }

    a_pma->bitmapSize = bitmapSize;
    for (size_t i = 0; i < bitmapSize; i++)
    {
        a_pma->bitmap[i] = (size_t) 0;
    }

    // if framesNumber is not multiple of WORDSIZE, marks the bits from bitmap
    // that exceed framesNumber.
    if (remainingBits != 0)
    {
        helper_MarksBits(a_pma, bitmapSize - 1, WORDSIZE - 1 - remainingBits,
                         WORDSIZE - remainingBits);
    }

    return ERROR_SUCCESS;
}

size_t PMM_Alloc(struct PhysicalMemoryAllocator *a_pma,
                 size_t a_framesNumber, size_t *a_physicalAddress)
{
    if (a_pma == NULL || a_physicalAddress == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    *a_physicalAddress = 0;

    if (a_pma->bitmap == NULL)
    {
        return ERROR_UNINITIALIZED;
    }

    if (a_framesNumber == 0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if (a_framesNumber > a_pma->freeFramesNumber)
    {
        return ERROR_NO_FREE_MEMORY;
    }

#define SET_PROCESSING          \
    if (processing == 0)        \
    {                           \
        processing = 1;         \
        i_start = i;            \
        j_start = j;            \
    }

#define RESET_PROCESSING                    \
    if (processing == 1)                    \
    {                                       \
        processing = 0;                     \
        i_start = 0;                        \
        j_start = 0;                        \
        requiredFrames = a_framesNumber;    \
    }

    //search from a_pma->positionLastAllocatedFrame to the last frame
    size_t requiredFrames = a_framesNumber;
    size_t i = a_pma->positionLastAllocatedFrame;
    size_t j = 0;
    size_t limit = a_pma->bitmapSize;
    size_t i_start = 0;
    size_t j_start = 0;
    uint8 processing = 0;
    uint8 keepSearching = 1;

Searching:
    for (; i < limit; i++)
    {
        // checks if the all bits are marked
        if (a_pma->bitmap[i] == (~((size_t) 0)))
        {
            RESET_PROCESSING;
            continue;
        }

        j = WORDSIZE;
        do
        {
            j--;
            if ((a_pma->bitmap[i] & (((size_t) 1) << j)) == 0)
            {
                SET_PROCESSING

                requiredFrames--;
                if (requiredFrames == 0)
                {
                    // marks bits
                    helper_MarksBits(a_pma, i_start, j_start, a_framesNumber);

                    // computes the physical address
                    size_t frames = i_start * WORDSIZE;
                    frames += (WORDSIZE - 1 - j_start);
                    *a_physicalAddress = frames * a_pma->frameSize;

                    // set the last position
                    a_pma->positionLastAllocatedFrame = i;

                    a_pma->freeFramesNumber -= a_framesNumber;

                    return ERROR_SUCCESS;
                }
            }
            else
            {
                RESET_PROCESSING;
            }
        } while (j > 0);
    }

    //search from the first frame to a_pma->positionLastAllocatedFrame
    if (keepSearching == 1)
    {
        keepSearching = 0;
        RESET_PROCESSING;
        i = 0;
        limit = a_pma->positionLastAllocatedFrame;

        goto Searching;
    }

    return ERROR_NOT_FOUND;

#undef SET_PROCESSING
#undef RESET_PROCESSING
}

size_t PMM_Free(struct PhysicalMemoryAllocator *a_pma,
                size_t a_framesNumber, size_t a_physicalAddress)
{
    if (a_pma == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_pma->bitmap == NULL)
    {
        return ERROR_UNINITIALIZED;
    }

    size_t framesNumber = (a_pma->endAddress - a_pma->startAddress) / a_pma->frameSize;

    if (a_framesNumber == 0                                     ||
        a_framesNumber > framesNumber                           ||
        (a_physicalAddress & (a_pma->frameSize - 1)) != 0       ||
        a_physicalAddress < a_pma->startAddress                 ||
        a_physicalAddress >= a_pma->endAddress)
    {
        return ERROR_INVALID_PARAMETER;
    }

    framesNumber = (a_pma->endAddress - a_physicalAddress) / a_pma->frameSize;
    if (framesNumber < a_framesNumber)
    {
        return ERROR_INVALID_STATE;
    }

    // computes indexBitmap and indexBits
    framesNumber = a_physicalAddress / a_pma->frameSize;
    size_t startIndexBitmap = framesNumber / WORDSIZE;
    size_t startIndexBits   = WORDSIZE - 1 - framesNumber % WORDSIZE;

    size_t indexBitmap = startIndexBitmap;
    size_t indexBits   = startIndexBits;

    // checks if the all a_framesNumber frames are marked
    framesNumber = a_framesNumber;
    indexBits++;
    do
    {
        do
        {
            indexBits--;

            // checks if bit is 0
            if ((a_pma->bitmap[indexBitmap] & (((size_t) 1) << indexBits)) == 0)
            {
                return ERROR_INVALID_STATE;
            }

            framesNumber--;
            if (framesNumber == 0)
            {
                goto FreesBits;
            }
        } while(indexBits != 0);

        indexBits = WORDSIZE;
        indexBitmap++;
    } while (true);

FreesBits:
    helper_FreesBits(a_pma, startIndexBitmap, startIndexBits, a_framesNumber);

    a_pma->freeFramesNumber += a_framesNumber;

    return ERROR_SUCCESS;
}
