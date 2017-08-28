#include <cut.h>

#include <stdlib.h>

#include "kernel/include/memory/bitmap_pma.h"
#include "kernel/include/memory/paa.h"

#define ALIGN(addr, align) addr = addr & (~(align - 1))
#define CHECK_ALIGN(addr, align) ((addr & (align - 1)) != 0)

CUT_DEFINE_TEST(test_bitmapCreate)
{
    struct BitmapPMA bpma = { 0 };
    uint32 err;

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x0,
        0x6400000 // 100 mibs
    );
    CUT_CHECK_OPERATOR_UINT32 (err,                             ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_UINT64 (bpma.startAddress,               ==, 0x0);
    CUT_CHECK_OPERATOR_UINT64 (bpma.endAddress,                 ==, 0x6400000);
    CUT_CHECK_OPERATOR_UINT32 (bpma.frameSize,                  ==, 0x1000);
    CUT_CHECK_OPERATOR_UINT32 (bpma.freeFramesNumber,           ==, 25600);
    CUT_CHECK_OPERATOR_UINT32 (bpma.positionLastAllocatedFrame, ==, 0);
    CUT_CHECK_OPERATOR_UINT32 (bpma.bitmapSize,                 ==, 400);
    CUT_CHECK_OPERATOR_ADDRESS(bpma.bitmap,                     !=, NULL);

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x200123, // ~2 mibs
        0x6400123 // ~100 mibs
    );
    CUT_CHECK_OPERATOR_UINT32 (err,                             ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_UINT64 (bpma.startAddress,               ==, 0x201000);
    CUT_CHECK_OPERATOR_UINT64 (bpma.endAddress,                 ==, 0x6400000);
    CUT_CHECK_OPERATOR_UINT32 (bpma.frameSize,                  ==, 0x1000);
    CUT_CHECK_OPERATOR_UINT32 (bpma.freeFramesNumber,           ==, 25087);
    CUT_CHECK_OPERATOR_UINT32 (bpma.positionLastAllocatedFrame, ==, 0);
    CUT_CHECK_OPERATOR_UINT32 (bpma.bitmapSize,                 ==, 392);
    CUT_CHECK_OPERATOR_ADDRESS(bpma.bitmap,                     !=, NULL);

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x1000,
        0x0
    );
    CUT_CHECK_OPERATOR_UINT32(err, ==, ERROR_INVALID_PARAMETER);

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x1000,
        0x1000
    );
    CUT_CHECK_OPERATOR_UINT32(err, ==, ERROR_INVALID_PARAMETER);

    err = BitmapPMA_createAllocator(
        NULL,
        0x1000,
        0x0,
        0x6400000 // 100 mibs
    );
    CUT_CHECK_OPERATOR_UINT32(err, ==, ERROR_NULL_POINTER);

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1001,
        0x0,
        0x6400000 // 100 mibs
    );
    CUT_CHECK_OPERATOR_UINT32(err, ==, ERROR_INVALID_PARAMETER);
}

CUT_DEFINE_TEST(test_bitmapAlloc)
{
    /*
     * first column: area size
     * second column: expected address
     * third column: expected free frames number after alloc()
     */
    static uint64 testCases[][3] = {
        0x00005000, 0x00000000, 25595,
        0x00000123, 0x00005000, 25594,
        0x00002012, 0x00006000, 25591,
        0x000F7000, 0x00009000, 25344,
        0x000FFFFF, 0x00100000, 25088,
        0x00200000, 0x00200000, 24576,
        0x003FE001, 0x00400000, 23553,
        0x00001000, 0x007FF000, 23552,
        0x05BFE123, 0x00800000,     1,
        0x00001000, 0x063FF000,     0
    };
    static uint32 testCasesLength = 10;

    struct BitmapPMA bpma = { 0 };
    uint32 err = 0;
    uint64 addr = 0;

    CUT_CHECK(BitmapPMA_alloc(&bpma, testCases[0][0], &addr) == ERROR_UNINITIALIZED);

    /*----- when startAddress is at 0x0 -----*/

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x0,
        0x6400000 // 100 mibs
    );
    CUT_CHECK(err == ERROR_SUCCESS);

    CUT_CHECK(BitmapPMA_alloc( NULL, 0x1000, &addr) == ERROR_NULL_POINTER);
    CUT_CHECK(BitmapPMA_alloc(&bpma, 0x1000,  NULL) == ERROR_NULL_POINTER);
    CUT_CHECK(BitmapPMA_alloc(&bpma, 0x0000, &addr) == ERROR_INVALID_PARAMETER);

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK(BitmapPMA_alloc(&bpma, testCases[i][0], &addr) == ERROR_SUCCESS);
        CUT_CHECK(addr == testCases[i][1]);
        CUT_CHECK(bpma.freeFramesNumber == testCases[i][2]);
    }

    CUT_CHECK(BitmapPMA_alloc(&bpma, 0x1, &addr) == ERROR_NO_FREE_MEMORY);

    free(bpma.bitmap);

    /*----- when startAddress is greater than 0x0 -----*/

    err = BitmapPMA_createAllocator(
        &bpma, 
        0x1000, 
        0x400000, // 4 mibs
        0x6800000); // 104 mibs
    CUT_CHECK(err == ERROR_SUCCESS);

    CUT_CHECK(BitmapPMA_alloc( NULL, 0x1000, &addr) == ERROR_NULL_POINTER);
    CUT_CHECK(BitmapPMA_alloc(&bpma, 0x1000,  NULL) == ERROR_NULL_POINTER);
    CUT_CHECK(BitmapPMA_alloc(&bpma, 0x0000, &addr) == ERROR_INVALID_PARAMETER);

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK(BitmapPMA_alloc(&bpma, testCases[i][0], &addr) == ERROR_SUCCESS);
        CUT_CHECK(addr == (testCases[i][1] + bpma.startAddress));
        CUT_CHECK(bpma.freeFramesNumber == testCases[i][2]);
    }

    CUT_CHECK(BitmapPMA_alloc(&bpma, 0x1, &addr) == ERROR_NO_FREE_MEMORY);

    free(bpma.bitmap);
}

CUT_DEFINE_TEST(test_bitmapFree)
{
    /*
     * first column: area size
     * second column: expected address
     * third column: expected free frames number after alloc()
     * fourth column: expected free frames number after free()
     */
    static uint64 testCases[][4] = {
        0x00005000, 0x00000000, 25595,     5,
        0x00000123, 0x00005000, 25594,     6,
        0x00002012, 0x00006000, 25591,     9,
        0x000F7000, 0x00009000, 25344,   256,
        0x000FFFFF, 0x00100000, 25088,   512,
        0x00200000, 0x00200000, 24576,  1024,
        0x003FE001, 0x00400000, 23553,  2047,
        0x00001000, 0x007FF000, 23552,  2048,
        0x05BFE123, 0x00800000,     1, 25599,
        0x00001000, 0x063FF000,     0, 25600
    };
    static uint32 testCasesLength = 10;

    struct BitmapPMA bpma = { 0 };
    uint32 err = 0;
    uint64 addr = 0;

    CUT_CHECK(BitmapPMA_free(&bpma, 0x1000, 0x0) == ERROR_UNINITIALIZED);

    /*----- when startAddress is at 0x0 -----*/

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x0,
        0x6400000 // 100 mibs
    );
    CUT_CHECK(err == ERROR_SUCCESS);

    CUT_CHECK(BitmapPMA_free( NULL, 0x00001000, 0x00001000          ) == ERROR_NULL_POINTER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00001000, bpma.startAddress   ) == ERROR_INVALID_STATE);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00002000, 0x063FF000          ) == ERROR_INVALID_STATE);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x06500000, 0x00001000          ) == ERROR_INVALID_PARAMETER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00000000, 0x00001000          ) == ERROR_INVALID_PARAMETER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00001000, bpma.endAddress + 1 ) == ERROR_INVALID_PARAMETER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00001000, 0x00000123          ) == ERROR_INVALID_PARAMETER);

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK(BitmapPMA_alloc(&bpma, testCases[i][0], &addr) == ERROR_SUCCESS);
        CUT_CHECK(addr == testCases[i][1]);
        CUT_CHECK(bpma.freeFramesNumber == testCases[i][2]);
    }

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK(BitmapPMA_free(&bpma, testCases[i][0], testCases[i][1]) == ERROR_SUCCESS);
        CUT_CHECK(bpma.freeFramesNumber == testCases[i][3]);
    }

    bpma.positionLastAllocatedFrame = 0;

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK(BitmapPMA_alloc(&bpma, testCases[i][0], &addr) == ERROR_SUCCESS);
        CUT_CHECK(addr == testCases[i][1]);
        CUT_CHECK(bpma.freeFramesNumber == testCases[i][2]);
    }

    free(bpma.bitmap);

    /*----- when startAddress is greater than 0x0 -----*/

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x400000, // 4 mibs
        0x6800000 // 104 mibs
    );
    CUT_CHECK(err == ERROR_SUCCESS);

    CUT_CHECK(BitmapPMA_free( NULL, 0x00001000, 0x00001000              ) == ERROR_NULL_POINTER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00001000, bpma.startAddress       ) == ERROR_INVALID_STATE);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00002000, 0x063FF000              ) == ERROR_INVALID_STATE);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x06500000, 0x00001000              ) == ERROR_INVALID_PARAMETER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00000000, 0x00001000              ) == ERROR_INVALID_PARAMETER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00001000, bpma.endAddress + 1     ) == ERROR_INVALID_PARAMETER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00001000, bpma.startAddress - 1   ) == ERROR_INVALID_PARAMETER);
    CUT_CHECK(BitmapPMA_free(&bpma, 0x00001000, 0x00000123              ) == ERROR_INVALID_PARAMETER);

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK(BitmapPMA_alloc(&bpma, testCases[i][0], &addr) == ERROR_SUCCESS);
        CUT_CHECK(addr == (testCases[i][1] + bpma.startAddress));
        CUT_CHECK(bpma.freeFramesNumber == testCases[i][2]);
    }

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK(BitmapPMA_free(&bpma, testCases[i][0], testCases[i][1]  + bpma.startAddress) == ERROR_SUCCESS);
        CUT_CHECK(bpma.freeFramesNumber == testCases[i][3]);
    }

    bpma.positionLastAllocatedFrame = 0;

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK(BitmapPMA_alloc(&bpma, testCases[i][0], &addr) == ERROR_SUCCESS);
        CUT_CHECK(addr == (testCases[i][1] + bpma.startAddress));
        CUT_CHECK(bpma.freeFramesNumber == testCases[i][2]);
    }

    free(bpma.bitmap);
}

CUT_DEFINE_TEST(test_bitmapReserve)
{
    /*
     * first column: area size
     * second column: start address for reserve()
     * third column: start address for free()
     * fourth column: expected free frames number after reserve()
     * fifth column: expected free frames number after free()
     */
    static uint64 testCases[][5] = {
        0x00004ffb, 0x00000005, 0x00000000, 25595,     5,
        0x00000120, 0x00005003, 0x00005000, 25594,     6,
        0x00002005, 0x00006007, 0x00006000, 25591,     9,
        0x000F6ff7, 0x00009009, 0x00009000, 25344,   256,
        0x000FFFF4, 0x0010000b, 0x00100000, 25088,   512,
        0x001ffff3, 0x0020000d, 0x00200000, 24576,  1024,
        0x003FE001, 0x00400000, 0x00400000, 23553,  2047,
        0x00001000, 0x007FF000, 0x007FF000, 23552,  2048,
        0x05BFE123, 0x00800000, 0x00800000,     1, 25599,
        0x00001000, 0x063FF000, 0x063FF000,     0, 25600
    };
    static uint32 testCasesLength = 10;

    struct BitmapPMA bpma = { 0 };
    uint32 err;

    CUT_CHECK(BitmapPMA_reserve(&bpma, 0x1000, 0x0) == ERROR_UNINITIALIZED);

    /*----- when startAddress is at 0x0 -----*/

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x0,
        0x6400000 // 100 mibs
    );
    CUT_CHECK(err == ERROR_SUCCESS);

    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(NULL, 0x1000, 0x0),                  ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, 0x0, 0x0),                    ==, ERROR_INVALID_PARAMETER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, 0x1000, bpma.endAddress + 1), ==, ERROR_INVALID_PARAMETER);

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, testCases[i][0], testCases[i][1]), ==, ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(bpma.freeFramesNumber,                                      ==, testCases[i][3]);
    }

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(BitmapPMA_free(&bpma, testCases[i][0], testCases[i][2]), ==, ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(bpma.freeFramesNumber,                                   ==, testCases[i][4]);
    }
    
    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, testCases[i][0], testCases[i][1]), ==, ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(bpma.freeFramesNumber,                                      ==, testCases[i][3]);
    }

    free(bpma.bitmap);

    /*----- when startAddress is greater than 0x0 -----*/

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x400000, // 4 mibs
        0x6800000 // 104 mibs
    );
    CUT_CHECK(err == ERROR_SUCCESS);

    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(NULL, 0x1000, 0x0),                    ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, 0x0, 0x0),                      ==, ERROR_INVALID_PARAMETER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, 0x1000, bpma.startAddress - 1), ==, ERROR_INVALID_PARAMETER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, 0x1000, bpma.endAddress + 1),   ==, ERROR_INVALID_PARAMETER);

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, testCases[i][0], testCases[i][1] + bpma.startAddress), ==, ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(bpma.freeFramesNumber,                                                          ==, testCases[i][3]);
    }

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(BitmapPMA_free(&bpma, testCases[i][0], testCases[i][2] + bpma.startAddress), ==, ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(bpma.freeFramesNumber,                                                       ==, testCases[i][4]);
    }
    
    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, testCases[i][0], testCases[i][1] + bpma.startAddress), ==, ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(bpma.freeFramesNumber,                                                          ==, testCases[i][3]);
    }

    free(bpma.bitmap);
}

CUT_DEFINE_TEST(test_bitmapCheck)
{
    /*
     * first column: area size
     * second column: address where to reserve
     */
    static uint64 areas[][2] = {
        0x00005000, 0x00000000,
        0x000F7000, 0x00009000,
        0x00200000, 0x00200000,
        0x00001000, 0x007FF000,
        0x00001000, 0x063FF000
    };
    static uint32 areasLength = 5;

    /*
     * first column: zone for checking end
     * second column: zone for checking start
     * third column: status
     */
    static uint64 testCases[][3] = {
        0x00005000, 0x00000003, 1,
        0x00100000, 0x00009004, 1,
        0x00400000, 0x00200001, 1,
        0x00400000, 0x00000001, 1,
        0x06300000, 0x00000001, 1,
        0x00007000, 0x00005001, 0,
        0x00500000, 0x00400002, 0,
        0x00900000, 0x00800003, 0,
        0x00550000, 0x00500000, 0,
        0x00650000, 0x00600000, 0,
    };
    static uint32 testCasesLength = 10;
    static uint8 state = 0;

    /**
     * Shiftting intervals are used for testing BitmapPMA_check() correctness.
     * After checking one zone, we are shiffting it with one position backwards or upwards.
     * The status must remain the same.
     */
    static uint64 shiftInterval[][2] = {
         0,  0,
         1,  0,
         0,  1,
        -1,  0,
         0, -1,
         1, -1,
        -1,  1,
         1,  1,
        -1, -1
    };
    static uint32 shiftsLenght = 9;

    struct BitmapPMA bpma = { 0 };
    uint32 err;

    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, 0x0, 0x1000, &state), ==, ERROR_UNINITIALIZED);

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x0,
        0x6400000 // 100 mibs
    );
    CUT_CHECK(err == ERROR_SUCCESS);

    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(NULL, 0x0, 0x1000, &state), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, 0x1000, 0x0, &state), ==, ERROR_INVALID_PARAMETER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, 0x0, 0xFFFFFFFF, &state), ==, ERROR_INVALID_PARAMETER);
    state = 2;
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, 0x400000, 0x500000, &state), ==, ERROR_INVALID_PARAMETER);

    for (uint32 i = 0; i < areasLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, areas[i][0], areas[i][1]), ==, ERROR_SUCCESS);
    }

    for (uint32 i = 0; i < testCasesLength; i++) {
        for (uint32 j = 0; j < shiftsLenght; j++) {
            state = testCases[i][2];
            CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, testCases[i][1] + shiftInterval[j][0], testCases[i][0] + shiftInterval[j][1], &state), ==, ERROR_SUCCESS);
            CUT_CHECK_OPERATOR_UINT8(state, ==, 1);
        }

        for (uint32 j = 0; j < shiftsLenght; j++) {
            state = !testCases[i][2];
            CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, testCases[i][1] + shiftInterval[j][0], testCases[i][0] + shiftInterval[j][1], &state), ==, ERROR_SUCCESS);
            CUT_CHECK_OPERATOR_UINT8(state, ==, 0);
        }
    }

    free(bpma.bitmap);

    err = BitmapPMA_createAllocator(
        &bpma,
        0x1000,
        0x400000, // 4 mibs
        0x6800000 // 104 mibs
    );
    CUT_CHECK(err == ERROR_SUCCESS);

    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(NULL, 0x0, 0x1000, &state), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, 0x1000, 0x0, &state), ==, ERROR_INVALID_PARAMETER);
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, 0x0, 0xFFFFFFFF, &state), ==, ERROR_INVALID_PARAMETER);
    state = 2;
    CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, 0x400000, 0x500000, &state), ==, ERROR_INVALID_PARAMETER);


    for (uint32 i = 0; i < areasLength; i++) {
        areas[i][1] += bpma.startAddress;
    }

    for (uint32 i = 0; i < testCasesLength; i++) {
        testCases[i][0] += bpma.startAddress;
        testCases[i][1] += bpma.startAddress;
    }

    for (uint32 i = 0; i < areasLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(BitmapPMA_reserve(&bpma, areas[i][0], areas[i][1]), ==, ERROR_SUCCESS);
    }

    for (uint32 i = 0; i < testCasesLength; i++) {
        for (uint32 j = 0; j < shiftsLenght; j++) {
            state = testCases[i][2];
            CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, testCases[i][1] + shiftInterval[j][0], testCases[i][0] + shiftInterval[j][1], &state), ==, ERROR_SUCCESS);
            CUT_CHECK_OPERATOR_UINT8(state, ==, 1);
        }

        for (uint32 j = 0; j < shiftsLenght; j++) {
            state = !testCases[i][2];
            CUT_CHECK_OPERATOR_UINT32(BitmapPMA_check(&bpma, testCases[i][1] + shiftInterval[j][0], testCases[i][0] + shiftInterval[j][1], &state), ==, ERROR_SUCCESS);
            CUT_CHECK_OPERATOR_UINT8(state, ==, 0);
        }
    }

    free(bpma.bitmap);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_bitmapCreate);
    CUT_CALL_TEST(test_bitmapAlloc);
    CUT_CALL_TEST(test_bitmapFree);
    CUT_CALL_TEST(test_bitmapReserve);
    CUT_CALL_TEST(test_bitmapCheck);
CUT_END_MAIN
