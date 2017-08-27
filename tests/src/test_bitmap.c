#include <cut.h>

#include <stdlib.h>

#include "kernel/include/memory/bitmap_pma.h"
#include "kernel/include/memory/paa.h"

#define ALIGN(addr, align) addr = addr & (~(align - 1))
#define CHECK_ALIGN(addr, align) ((addr & (align - 1)) != 0)

void *bpma_memory;
struct BitmapPMA bpma;
uint64 addr1, addr2, addr3;
uint64 start, end;

CUT_DEFINE_TEST(test_bitmapCreate)
{
    uint32 err;
    bpma_memory = malloc(0x1000 * 50);

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    (uint64) bpma_memory, 
                                    (uint64) ((uint64) bpma_memory + 0x1000 * 50));
    CUT_CHECK(err == ERROR_SUCCESS);

    start = (uint64) bpma_memory;
    if (CHECK_ALIGN(start, 0x1000)) {
        ALIGN(start, 0x1000);
        start += 0x1000;
    }
    end = (uint64) bpma_memory + 0x1000 * 50;
    if (CHECK_ALIGN(end, 0x1000)) {
        ALIGN(end, 0x1000);
    }
    CUT_CHECK(bpma.freeFramesNumber == (end - start) / 0x1000);
    CUT_CHECK(bpma.bitmap != NULL);
}

CUT_DEFINE_TEST(test_bitmapAlloc)
{
    uint32 err;
    addr3 = 0x0;

    err = BitmapPMA_alloc(&bpma, 0, &addr1);
    CUT_CHECK(err == ERROR_INVALID_PARAMETER);
    err = BitmapPMA_alloc(NULL, 0x3000, &addr2);
    CUT_CHECK(err == ERROR_NULL_POINTER);
    err = BitmapPMA_alloc(&bpma, 0x5000, &addr3);
    CUT_CHECK(err == ERROR_SUCCESS && addr3 != 0x0);
}

CUT_DEFINE_TEST(test_bitmapFree)
{
    uint32 err;
    err = BitmapPMA_free(&bpma, 0x5000, addr3);
    CUT_CHECK(err == ERROR_SUCCESS);

    err = BitmapPMA_free(&bpma, 0x5000, addr3);
    CUT_CHECK(err == ERROR_INVALID_STATE);
}

CUT_DEFINE_TEST(test_bitmapReserve)
{
    uint32 err;
    err = BitmapPMA_reserve(&bpma, 0x5000, start);
    CUT_CHECK(err == ERROR_SUCCESS);

    err = BitmapPMA_free(&bpma, 0x5000, start);
    CUT_CHECK(err == ERROR_SUCCESS);
}

CUT_DEFINE_TEST(test_bitmapCheck)
{
    uint32 err;
    uint8 state = 0;
    err = BitmapPMA_check(&bpma, start, end, &state);
    CUT_CHECK(err == ERROR_SUCCESS && state);

    BitmapPMA_alloc(&bpma, 0x5000, &addr3);
    state = 1;
    err = BitmapPMA_check(&bpma, addr3, addr3 + 0x5000, &state);
    CUT_CHECK(err == ERROR_SUCCESS && state);

    BitmapPMA_free(&bpma, 0x5000, addr3);
    state = 0;
    err = BitmapPMA_check(&bpma, addr3, addr3 + 0x5000, &state);
    CUT_CHECK(err == ERROR_SUCCESS && state);
}

CUT_DEFINE_TEST(test_bitmapAll)
{
    uint32 err;
    uint8 state;

    //err = BitmapPMA_reserve(&bpma, 0x5000, start);
    //CUT_CHECK(err == ERROR_SUCCESS);

    //state = 1;
    //err = BitmapPMA_check(&bpma, start, start + 0x5000, &state);
    //CUT_CHECK(err == ERROR_SUCCESS && state);

    err = BitmapPMA_alloc(&bpma, 0x5000, &addr1);
    CUT_CHECK((err == ERROR_SUCCESS) && (addr1 == start));
    printf("%x\n", addr1);
    printf("%x\n", start);
    int tmp = ((unsigned int)addr1 == (unsigned int)start);
    printf("%d\n", tmp);

    err = BitmapPMA_alloc(&bpma, 0x2000, &addr2);
    printf("%x\n", addr2);
    CUT_CHECK(err == ERROR_SUCCESS && addr2 == addr1 + 0x5000);
    
    err = BitmapPMA_free(&bpma, 0x5000, addr1);
    CUT_CHECK(err == ERROR_SUCCESS);

    state = 1;
    err = BitmapPMA_check(&bpma, addr2, addr2 + 0x2000, &state);
    CUT_CHECK(err == ERROR_SUCCESS && state);

    err = BitmapPMA_alloc(&bpma, 0x2000, &addr3);
    CUT_CHECK(err == ERROR_SUCCESS && addr3 == start + 0x5000);

    err = BitmapPMA_free(&bpma, 0x2000, addr2);
    CUT_CHECK(err == ERROR_SUCCESS);

    state = 0;
    err = BitmapPMA_check(&bpma, addr2, addr2 + 0x2000, &state);
    CUT_CHECK(err == ERROR_SUCCESS && state);
    

    free(bpma.bitmap);
    free(bpma_memory);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_bitmapCreate);
    CUT_CALL_TEST(test_bitmapAlloc);
    CUT_CALL_TEST(test_bitmapFree);
    //CUT_CALL_TEST(test_bitmapReserve);
    CUT_CALL_TEST(test_bitmapCheck);
    CUT_CALL_TEST(test_bitmapAll);
CUT_END_MAIN
