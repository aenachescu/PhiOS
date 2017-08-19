#include <cut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kernel/include/memory/bitmap_pma.h"
#include "kernel/include/memory/paa.h"
#include "errors.h"

void *paa_memory;
void *bpma_memory;

size_t PAA_alloc(size_t a_size, size_t *a_address, size_t a_alignment)
{
    if (a_size == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (a_address == NULL) {
        return ERROR_NULL_POINTER;
    }
    *a_address = 0;

    size_t g_placementAddress = (size_t) malloc(a_size + a_alignment - 1);
    if ((g_placementAddress & (a_alignment - 1)) != 0) {
        g_placementAddress &= (~(a_alignment - 1));
        g_placementAddress += a_alignment;
    }

    *a_address = g_placementAddress;
    g_placementAddress += a_size;

    return ERROR_SUCCESS;
}

CUT_DEFINE_TEST(test_bitmapCreate)
{
    uint32 err;
    paa_memory = malloc(0x1000 * 25);
    bpma_memory = malloc(0x1000 * 50);

    struct BitmapPMA bpma;
    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    (uint64) bpma_memory, 
                                    (uint64) ((uint64) bpma_memory + 0x1000 * 50));
    CUT_CHECK(err == ERROR_SUCCESS);
}

CUT_DEFINE_TEST(test_bitmapAlloc)
{

}

CUT_DEFINE_TEST(test_bitmapFree)
{

}

CUT_DEFINE_TEST(test_bitmapReserve)
{

}

CUT_DEFINE_TEST(test_bitmapCheck)
{
    free(paa_memory);
    free(bpma_memory);
}
