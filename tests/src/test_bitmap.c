#include <cut.h>

#include <stdlib.h>

#include "kernel/include/memory/bitmap_pma.h"
#include "kernel/include/memory/paa.h"

#define ALIGN(addr, align) addr = addr & (~(align - 1))
#define CHECK_ALIGN(addr, align) ((addr & (align - 1)) != 0)

CUT_DEFINE_TEST(test_bitmapCreate)
{
    struct BitmapPMA bpma;
    uint32 err;

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x0, 
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x1000, 
                                    0x0); // 100 mibs
    CUT_CHECK(err == ERROR_INVALID_PARAMETER);

    err = BitmapPMA_createAllocator(NULL, 
                                    0x1000, 
                                    0x0, 
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_NULL_POINTER);

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1001, 
                                    0x0, 
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_INVALID_PARAMETER);
}

CUT_DEFINE_TEST(test_bitmapAlloc)
{
    struct BitmapPMA bpma;
    uint32 err;

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x0, 
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    uint64 addr;
    err = BitmapPMA_alloc(NULL, 0x1000, &addr);
    CUT_CHECK(err == ERROR_NULL_POINTER);
    err = BitmapPMA_alloc(&bpma, 0, &addr);
    CUT_CHECK(err == ERROR_INVALID_PARAMETER);
    err = BitmapPMA_alloc(&bpma, 0x1000, NULL);
    CUT_CHECK(err == ERROR_NULL_POINTER);

    err = BitmapPMA_alloc(&bpma, 0x5000, &addr);
    CUT_CHECK(err == ERROR_SUCCESS && addr == 0x0);
    err = BitmapPMA_alloc(&bpma, 0x4000, &addr);
    CUT_CHECK(err == ERROR_SUCCESS && addr == 0x5000);
    err = BitmapPMA_alloc(&bpma, 0x6400000 - 0x10000, &addr);
    CUT_CHECK(err == ERROR_SUCCESS);
    err = BitmapPMA_alloc(&bpma, 0x10000, &addr);
    CUT_CHECK(err == ERROR_NO_FREE_MEMORY);

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x400000, // 4 mibs
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    err = BitmapPMA_alloc(NULL, 0x1000, &addr);
    CUT_CHECK(err == ERROR_NULL_POINTER);
    err = BitmapPMA_alloc(&bpma, 0, &addr);
    CUT_CHECK(err == ERROR_INVALID_PARAMETER);
    err = BitmapPMA_alloc(&bpma, 0x1000, NULL);
    CUT_CHECK(err == ERROR_NULL_POINTER);

    err = BitmapPMA_alloc(&bpma, 0x5000, &addr);
    CUT_CHECK(err == ERROR_SUCCESS && addr == 0x400000);
    err = BitmapPMA_alloc(&bpma, 0x4000, &addr);
    CUT_CHECK(err == ERROR_SUCCESS && addr == 0x405000);
    err = BitmapPMA_alloc(&bpma, 0x6400000 - 0x400000 - 0x10000, &addr);
    CUT_CHECK(err == ERROR_SUCCESS);
    err = BitmapPMA_alloc(&bpma, 0x10000, &addr);
    CUT_CHECK(err == ERROR_NO_FREE_MEMORY);

    free(bpma.bitmap);
}

CUT_DEFINE_TEST(test_bitmapFree)
{
    struct BitmapPMA bpma;
    uint32 err;

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x0, 
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x400000, // 4 mibs
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);
}

CUT_DEFINE_TEST(test_bitmapReserve)
{
    struct BitmapPMA bpma;
    uint32 err;

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x0, 
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x400000, // 4 mibs
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);
}

CUT_DEFINE_TEST(test_bitmapCheck)
{
    struct BitmapPMA bpma;
    uint32 err;

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x0, 
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x400000, // 4 mibs
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);
}

CUT_DEFINE_TEST(test_bitmapAll)
{
    struct BitmapPMA bpma;
    uint32 err;

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x0, 
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(&bpma, 
                                    0x1000, 
                                    0x400000, // 4 mibs
                                    0x6400000); // 100 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    free(bpma.bitmap);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_bitmapCreate);
    CUT_CALL_TEST(test_bitmapAlloc);
    CUT_CALL_TEST(test_bitmapFree);
    CUT_CALL_TEST(test_bitmapReserve);
    CUT_CALL_TEST(test_bitmapCheck);
    CUT_CALL_TEST(test_bitmapAll);
CUT_END_MAIN
