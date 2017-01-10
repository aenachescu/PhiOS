#include "bitmap_pma.h"
#include "paa.h"


/*
 * This function doesn't checks if the input is correct because is a helper
 * function and it expects the input to be correct.
 */
static void helper_marksBits(struct BitmapPMA *a_bpma,
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
            a_bpma->bitmap[indexBitmap] |= (((size_t) 1) << indexBits);

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
 static void helper_freesBits(struct BitmapPMA *a_bpma,
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
            a_bpma->bitmap[indexBitmap] &= (~(((size_t) 1) << indexBits));

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

size_t BitmapPMA_createAllocator(struct BitmapPMA *a_bpma, size_t a_frameSize,
                                size_t a_startAddress, size_t a_endAddress)
{
    if (a_bpma == NULL)
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

    a_bpma->startAddress                 = a_startAddress;
    a_bpma->endAddress                   = a_endAddress;
    a_bpma->frameSize                    = a_frameSize;
    a_bpma->freeFramesNumber             = (a_endAddress - a_startAddress) / a_frameSize;
    a_bpma->positionLastAllocatedFrame   = 0;
    a_bpma->bitmap                       = NULL;
    a_bpma->bitmapSize                   = 0;

    size_t framesNumber     = (a_endAddress - a_startAddress) / a_frameSize;
    size_t bitmapSize       = framesNumber / WORDSIZE;
    size_t remainingBits    = framesNumber % WORDSIZE;

    if (remainingBits != 0)
    {
        bitmapSize++;
    }

    size_t error = PAA_alloc(sizeof(size_t) * bitmapSize,
                             (size_t*) &a_bpma->bitmap,
                             WORDSIZE);

    if (error != ERROR_SUCCESS)
    {
        a_bpma->bitmap       = NULL;
        a_bpma->bitmapSize   = 0;

        return error;
    }

    a_bpma->bitmapSize = bitmapSize;
    for (size_t i = 0; i < bitmapSize; i++)
    {
        a_bpma->bitmap[i] = (size_t) 0;
    }

    // if framesNumber is not multiple of WORDSIZE, marks the bits from bitmap
    // that exceed framesNumber.
    if (remainingBits != 0)
    {
        helper_marksBits(a_bpma, bitmapSize - 1, WORDSIZE - 1 - remainingBits,
                         WORDSIZE - remainingBits);
    }

    return ERROR_SUCCESS;
}

size_t BitmapPMA_alloc(struct BitmapPMA *a_bpma,
                       size_t a_framesNumber, size_t *a_physicalAddress)
{
    if (a_bpma == NULL || a_physicalAddress == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    *a_physicalAddress = 0;

    if (a_bpma->bitmap == NULL)
    {
        return ERROR_UNINITIALIZED;
    }

    if (a_framesNumber == 0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if (a_framesNumber > a_bpma->freeFramesNumber)
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

    //search from a_bpma->positionLastAllocatedFrame to the last frame
    size_t requiredFrames = a_framesNumber;
    size_t i = a_bpma->positionLastAllocatedFrame;
    size_t j = 0;
    size_t limit = a_bpma->bitmapSize;
    size_t i_start = 0;
    size_t j_start = 0;
    uint8 processing = 0;
    uint8 keepSearching = 1;

Searching:
    for (; i < limit; i++)
    {
        // checks if the all bits are marked
        if (a_bpma->bitmap[i] == (~((size_t) 0)))
        {
            RESET_PROCESSING;
            continue;
        }

        j = WORDSIZE;
        do
        {
            j--;
            if ((a_bpma->bitmap[i] & (((size_t) 1) << j)) == 0)
            {
                SET_PROCESSING

                requiredFrames--;
                if (requiredFrames == 0)
                {
                    // marks bits
                    helper_marksBits(a_bpma, i_start, j_start, a_framesNumber);

                    // computes the physical address
                    size_t frames = i_start * WORDSIZE;
                    frames += (WORDSIZE - 1 - j_start);
                    *a_physicalAddress = frames * a_bpma->frameSize;

                    // set the last position
                    a_bpma->positionLastAllocatedFrame = i;

                    a_bpma->freeFramesNumber -= a_framesNumber;

                    return ERROR_SUCCESS;
                }
            }
            else
            {
                RESET_PROCESSING;
            }
        } while (j > 0);
    }

    //search from the first frame to a_bpma->positionLastAllocatedFrame
    if (keepSearching == 1)
    {
        keepSearching = 0;
        RESET_PROCESSING;
        i = 0;
        limit = a_bpma->positionLastAllocatedFrame;

        goto Searching;
    }

    return ERROR_NOT_FOUND;

#undef SET_PROCESSING
#undef RESET_PROCESSING
}

size_t BitmapPMA_free(struct BitmapPMA *a_bpma,
                      size_t a_framesNumber, size_t a_physicalAddress)
{
    if (a_bpma == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_bpma->bitmap == NULL)
    {
        return ERROR_UNINITIALIZED;
    }

    size_t framesNumber = (a_bpma->endAddress - a_bpma->startAddress) / a_bpma->frameSize;

    if (a_framesNumber == 0                                     ||
        a_framesNumber > framesNumber                           ||
        (a_physicalAddress & (a_bpma->frameSize - 1)) != 0       ||
        a_physicalAddress < a_bpma->startAddress                 ||
        a_physicalAddress >= a_bpma->endAddress)
    {
        return ERROR_INVALID_PARAMETER;
    }

    framesNumber = (a_bpma->endAddress - a_physicalAddress) / a_bpma->frameSize;
    if (framesNumber < a_framesNumber)
    {
        return ERROR_INVALID_STATE;
    }

    // computes indexBitmap and indexBits
    framesNumber = a_physicalAddress / a_bpma->frameSize;
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
            if ((a_bpma->bitmap[indexBitmap] & (((size_t) 1) << indexBits)) == 0)
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
    helper_freesBits(a_bpma, startIndexBitmap, startIndexBits, a_framesNumber);

    a_bpma->freeFramesNumber += a_framesNumber;

    return ERROR_SUCCESS;
}
