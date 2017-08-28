#include <cut.h>

#include <string.h>
#include <stdlib.h>

#include "kernel/include/memory/paa.h"

CUT_DEFINE_TEST(test_PAA)
{
    uint32 err = ERROR_SUCCESS;
    uint64 addr = 0;

    CUT_CHECK_OPERATOR_UINT32(PAA_init(0), ==, ERROR_INVALID_PARAMETER);

    CUT_CHECK_OPERATOR_UINT32(PAA_alloc(0x1000, &addr, 0x1000), ==, ERROR_UNINITIALIZED);

    CUT_CHECK_OPERATOR_UINT32(PAA_init(0x1000), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_UINT64(PAA_getCurrentAddress(), ==, 0x1000);

    CUT_CHECK_OPERATOR_UINT32(PAA_init(0x1000), ==, ERROR_ALREADY_INITIALIZED);

    CUT_CHECK_OPERATOR_UINT32(PAA_alloc(0, &addr, 0x1000), ==, ERROR_INVALID_PARAMETER);
    CUT_CHECK_OPERATOR_UINT32(PAA_alloc(0x1000, &addr, 0), ==, ERROR_INVALID_PARAMETER);
    CUT_CHECK_OPERATOR_UINT32(PAA_alloc(0x1000, NULL, 0x1000), ==, ERROR_NULL_POINTER);

    /*
     * first column: area size
     * second column: align
     * third column: expected address
     */
    static uint64 testCases[][3] = {
        0x00000010, 0x00000002, 0x00001000,
        0x00000123, 0x00001000, 0x00002000,
        0x00000123, 0x00000010, 0x00002130,
        0x00001234, 0x00000001, 0x00002253,
        0x00001000, 0x00100000, 0x00100000
    };
    static uint32 testCasesLength = 5;

    for (uint32 i = 0; i < testCasesLength; i++) {
        CUT_CHECK_OPERATOR_UINT32(PAA_alloc(testCases[i][0], &addr, testCases[i][1]), ==, ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT64(addr, ==, testCases[i][2]);
    }
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_PAA);
CUT_END_MAIN
