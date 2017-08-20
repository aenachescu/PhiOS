#include <cut.h>

#include <stdlib.h>

#include "kernel/include/memory/bitmap_pma.h"
#include "kernel/include/memory/paa.h"

void *paa_memory;
void *bpma_memory;

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

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_bitmapCreate);
    CUT_CALL_TEST(test_bitmapAlloc);
    CUT_CALL_TEST(test_bitmapFree);
    CUT_CALL_TEST(test_bitmapReserve);
    CUT_CALL_TEST(test_bitmapCheck);
CUT_END_MAIN
