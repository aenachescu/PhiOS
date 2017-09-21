#include "tests/asserts.h"

#include <cut.h>

#include "util/kstdlib/include/ksnprintf_helper.h"
#include "util/kstdlib/include/ksnprintf.h"
#include "util/kstdlib/include/kvsnprintf.h"
#include "util/kstdlib/include/kstring.h"

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

uint32 call_kvsnprintf(
    char *a_buffer,
    uint32 *a_length,
    const char *a_format,
    ...)
{
    va_list arg;
    va_start(arg, a_format);

    uint32 retValue = kvsnprintf(a_buffer, a_length, a_format, arg);

    va_end(arg);

    return retValue;
}

CUT_DEFINE_TEST(test_kvsnprintf)
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

    CUT_CHECK_OPERATOR_UINT32(call_kvsnprintf(NULL, &length, "Result: %d", 1), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(call_kvsnprintf(buffer, NULL, "Result: %d", 1), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(call_kvsnprintf(buffer, &length, NULL), ==, ERROR_NULL_POINTER);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        length = (uint32) testCases[i].length;

        CUT_CHECK_OPERATOR_UINT32(
            call_kvsnprintf(
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

uint32 call_ksnprintf_helper(
    char *a_buffer,
    uint32 a_length,
    const char *a_format,
    ...)
{
    va_list arg;
    va_start(arg, a_format);

    uint32 retValue = ksnprintf_helper(a_buffer, a_length, a_format, arg);

    va_end(arg);

    return retValue;
}

#define CHECK(buffer, length, format, expectedBuffer, computeExpectedLength, ...)  \
    if (computeExpectedLength == true) {                                    \
        kstrlen(expectedBuffer, &expectedLength);                           \
        expectedLength++;                                                   \
    }                                                                       \
    CUT_CHECK_OPERATOR_UINT32(                                              \
        call_ksnprintf_helper(                                              \
            buffer,                                                         \
            length,                                                         \
            format,                                                         \
            ##__VA_ARGS__                                                   \
        ),                                                                  \
        ==,                                                                 \
        (uint32) expectedLength                                             \
    );                                                                      \
    CUT_CHECK_OPERATOR_UINT32(                                              \
        kstrcmp(                                                            \
            buffer,                                                         \
            expectedBuffer,                                                 \
            &result                                                         \
        ),                                                                  \
        ==,                                                                 \
        ERROR_SUCCESS                                                       \
    );                                                                      \
    CUT_CHECK_OPERATOR_INT32(result, ==, 0);

CUT_DEFINE_TEST(test_ksnprintf_helper_d_specifier)
{
    char buffer[512];
    sint32 result;
    size_t expectedLength = 0;

    CHECK(
        buffer,
        512,
        "N1: %d %d %+d %+d % d % d\n",
        "N1: 5 -3 +5 -3  12345 -12345\n",
        true,
        5, -3, 5, -3, 12345, -12345
    );

    kstrlen("Numbers: 1 2 3", &expectedLength);
    expectedLength++;
    CHECK(
        buffer,
        13,
        "Numbers: %d %d %d",
        "Numbers: 1 2",
        false,
        1, 2, 3
    );

    CHECK(
        buffer,
        512,
        "Numbers: %05d %-5d %5d %0*d %-*d %*d",
        "Numbers: 00001 2         3 01 2   3",
        true,
        1, 2, 3, 2, 1, 2, 2, 2, 3
    )

    CHECK(
        buffer,
        512,
        "Numbers: %hhd %hhd %hd %hd %ld %ld %lld %lld",
        "Numbers: 12 -67 12345 -12345 123456 -654321 1234567890 -1234567890",
        true,
        12, -67,
        12345, -12345,
        123456, -654321,
        1234567890LL, -1234567890LL
    )
}

CUT_DEFINE_TEST(test_ksnprintf_without_specifier)
{
    char buffer[512];
    sint32 result;
    size_t expectedLength = 0;

    CHECK(
        buffer,
        512,
        "string string",
        "string string",
        true
    );

    kstrlen("string string", &expectedLength);
    expectedLength++;
    CHECK(
        buffer,
        11,
        "string string",
        "string str",
        false
    );
}

CUT_DEFINE_TEST(test_ksnprintf_u_specifier)
{
    char buffer[512];
    sint32 result;
    size_t expectedLength = 0;

    CHECK(
        buffer,
        512,
        "Numbers: [%u] [%5u] [%05u] [%+u] [%-5u] [%-05u] [% 5u] [% 05u] [%*u]",
        "Numbers: [1] [    2] [00003] [4] [5    ] [6    ] [    7] [00008] [    9]",
        true,
        1, 2, 3, 4, 5, 6, 7, 8, 5, 9
    );

    CHECK(
        buffer,
        512,
        "Numbers: [%hhu] [%hhu] [%hu] [%u] [%lu] [%llu] [%5llu] [%5llu]",
        "Numbers: [1] [1] [2] [3] [4] [5] [123456] [  123]",
        true,
        1, 257, 2, 3, 4, 5ULL, 123456ULL, 123ULL
    );
}

CUT_DEFINE_MODULE(module_ksnprintf)
    CUT_CALL_TEST(test_ksnprintf);

    CUT_CALL_TEST(test_kvsnprintf);

    CUT_CALL_TEST(test_ksnprintf_helper_d_specifier);
    CUT_CALL_TEST(test_ksnprintf_without_specifier);
    CUT_CALL_TEST(test_ksnprintf_u_specifier);
CUT_END_MODULE
