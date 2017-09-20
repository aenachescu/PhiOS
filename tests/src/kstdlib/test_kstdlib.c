#include "tests/asserts.h"

#include <cut.h>

#include "util/kstdlib/include/kstdlib.h"

#define _countof(x) (sizeof(x) / sizeof(x[0]))

uint32 kernel_random32()
{
    return 0;
}

CUT_DEFINE_TEST(test_kitoa)
{
    struct TestCase {
        char buffer[64];
        char expectedBuffer[64];
        size_t length;
        size_t expectedLength;
        size_t base;
        sint32 value;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        // base 10
        {"", "-3"         ,  3,  5, 10, -123       , ERROR_INSUFFICIENT_BUFFER},
        {"", "0"          , 64,  1, 10, 0          , ERROR_SUCCESS},
        {"", "123"        , 64,  3, 10, 123        , ERROR_SUCCESS},
        {"", "-123"       , 64,  4, 10, -123       , ERROR_SUCCESS},
        {"", "-2147483648", 64, 11, 10, -2147483648, ERROR_SUCCESS},
        {"", "2147483647" , 64, 10, 10, 2147483647 , ERROR_SUCCESS},

        // base 16
        {"", "B"       ,  2, 3, 16, 123        , ERROR_INSUFFICIENT_BUFFER},
        {"", "7B"      , 64, 2, 16, 123        , ERROR_SUCCESS},
        {"", "FFFFFF85", 64, 8, 16, -123       , ERROR_SUCCESS},
        {"", "80000000", 64, 8, 16, -2147483648, ERROR_SUCCESS},
        {"", "7FFFFFFF", 64, 8, 16, 2147483647 , ERROR_SUCCESS},
        {"", "0"       , 64, 1, 16, 0          , ERROR_SUCCESS},

        // base 2
        {"", "1010"                            ,  5, 33, 2, 4200000005, ERROR_INSUFFICIENT_BUFFER},
        {"", "00000000000000000000000000000000", 64, 32, 2, 0, ERROR_SUCCESS},
        {"", "00000000000000000000000001111011", 64, 32, 2, 123, ERROR_SUCCESS},
        {"", "11111111111111111111111110000101", 64, 32, 2, -123, ERROR_SUCCESS},
        {"", "10000000000000000000000000000000", 64, 32, 2, -2147483648, ERROR_SUCCESS},
        {"", "01111111111111111111111111111111", 64, 32, 2, 2147483647, ERROR_SUCCESS},
    };

    char buffer[64] = { 0 };
    size_t length = 0;
    sint32 result;

    CUT_CHECK_OPERATOR_UINT32(kitoa(0, NULL, &length, 10), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kitoa(0, buffer, NULL, 10), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kitoa(0, buffer, &length, 5), ==, ERROR_UNSUPPORTED);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kitoa(
                testCases[i].value,
                testCases[i].buffer,
                &testCases[i].length,
                testCases[i].base
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_UINT32(
            (uint32) testCases[i].length,
            ==,
            (uint32) testCases[i].expectedLength
        );

        CUT_CHECK_OPERATOR_UINT32(
            kmemcmp(
                testCases[i].buffer,
                testCases[i].expectedBuffer,
                testCases[i].expectedLength,
                &result
            ),
            ==,
            ERROR_SUCCESS
        );

        CUT_CHECK_OPERATOR_UINT32(result, ==, 0);
    }
}

CUT_DEFINE_TEST(test_kutoa)
{
    struct TestCase {
        char buffer[64];
        char expectedBuffer[64];
        size_t length;
        size_t expectedLength;
        size_t base;
        uint32 value;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        // base 10
        {"", "32"         ,  3,  4, 10, 123        , ERROR_INSUFFICIENT_BUFFER},
        {"", "0"          , 64,  1, 10, 0          , ERROR_SUCCESS},
        {"", "123"        , 64,  3, 10, 123        , ERROR_SUCCESS},
        {"", "4294967295" , 64, 10, 10, 4294967295 , ERROR_SUCCESS},

        // base 16
        {"", "B"       ,  2, 3, 16, 123        , ERROR_INSUFFICIENT_BUFFER},
        {"", "7B"      , 64, 2, 16, 123        , ERROR_SUCCESS},
        {"", "FFFFFFFF", 64, 8, 16, 4294967295 , ERROR_SUCCESS},
        {"", "0"       , 64, 1, 16, 0          , ERROR_SUCCESS},

        // base 2
        {"", "1010"                            ,  5, 33, 2, 4200000005 , ERROR_INSUFFICIENT_BUFFER},
        {"", "00000000000000000000000000000000", 64, 32, 2, 0          , ERROR_SUCCESS},
        {"", "00000000000000000000000001111011", 64, 32, 2, 123        , ERROR_SUCCESS},
        {"", "11111111111111111111111111111111", 64, 32, 2, 4294967295 , ERROR_SUCCESS},
    };

    char buffer[64] = { 0 };
    size_t length = 0;
    sint32 result;

    CUT_CHECK_OPERATOR_UINT32(kutoa(0, NULL, &length, 10), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kutoa(0, buffer, NULL, 10), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kutoa(0, buffer, &length, 5), ==, ERROR_UNSUPPORTED);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kutoa(
                testCases[i].value,
                testCases[i].buffer,
                &testCases[i].length,
                testCases[i].base
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_UINT32(
            (uint32) testCases[i].length,
            ==,
            (uint32) testCases[i].expectedLength
        );

        CUT_CHECK_OPERATOR_UINT32(
            kmemcmp(
                testCases[i].buffer,
                testCases[i].expectedBuffer,
                testCases[i].expectedLength,
                &result
            ),
            ==,
            ERROR_SUCCESS
        );

        CUT_CHECK_OPERATOR_UINT32(result, ==, 0);
    }
}

CUT_DEFINE_MODULE(module_kstdlib)
    CUT_CALL_TEST(test_kitoa);
    CUT_CALL_TEST(test_kutoa);
CUT_END_MODULE
