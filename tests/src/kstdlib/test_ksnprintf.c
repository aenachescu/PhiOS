#include "tests/asserts.h"

#include <cut.h>

#include "util/kstdlib/include/ksnprintf_helper.h"
#include "util/kstdlib/include/ksnprintf.h"
#include "util/kstdlib/include/kvsnprintf.h"

#define _countof(x) (sizeof(x) / sizeof(x[0]))

CUT_DEFINE_TEST(test_ksnprintf)
{
    struct TestCase {
        char buffer[256];
        char expectedBuffer[256];
        char format[32];
        sint32 value;
        size_t length;
        size_t expectedLength;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {"", "A1"         , "A%d"               , 1, 256,  3, ERROR_SUCCESS},
        {"", "2A"         , "%dA"               , 2, 256,  3, ERROR_SUCCESS},
        {"", "a3a"        , "a%da"              , 3, 256,  4, ERROR_SUCCESS},
        {"", "asdasd 4 "  , "asdasd %d asdasd"  , 4, 10 , 10, ERROR_INSUFFICIENT_BUFFER},
        {"", "qwertyuiop ", "qwertyuiop %d\n"   , 5, 12 , 12, ERROR_INSUFFICIENT_BUFFER},
        {"", "qw"         , "qwertyuiop %d\n"   , 6, 3  ,  3, ERROR_INSUFFICIENT_BUFFER},
    };

    char buffer[32];
    uint32 length;
    sint32 result;

    CUT_CHECK_OPERATOR_UINT32(ksnprintf(NULL, &length, "Result: %d", 1), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(ksnprintf(buffer, NULL, "Result: %d", 1), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(ksnprintf(buffer, &length, NULL), ==, ERROR_NULL_POINTER);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        length = (uint32) testCases[i].length;

        CUT_CHECK_OPERATOR_UINT32(
            ksnprintf(
                testCases[i].buffer,
                &length,
                testCases[i].format,
                testCases[i].value
            ),
            ==,
            testCases[i].retValue
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

        CUT_CHECK_OPERATOR_INT32(result, ==, 0);
    }
}

CUT_DEFINE_TEST(test_kvsnprintf)
{

}

CUT_DEFINE_MODULE(module_ksnprintf)
    CUT_CALL_TEST(test_ksnprintf);
    CUT_CALL_TEST(test_kvsnprintf);
CUT_END_MODULE
