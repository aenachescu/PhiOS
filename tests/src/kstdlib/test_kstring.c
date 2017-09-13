#include "tests/asserts.h"
#include <cut.h>

#include "util/kstdlib/include/kstring.h"

#include <string.h>

#define _countof(x) (sizeof(x) / sizeof(x[0]))

CUT_DEFINE_TEST(test_kmemchr)
{
    struct TestCase {
        const char *str;
        uint8 value;
        uint32 position;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {"asdfghjkl", 'a', 0, ERROR_SUCCESS},
        {"asdfghjkl", 'f', 3, ERROR_SUCCESS},
        {"asdfghjkl", 'l', 8, ERROR_SUCCESS}
    };

    void *result = NULL;

    CUT_CHECK_OPERATOR_UINT32(kmemchr(NULL, 'a', 5, &result), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemchr("as", 'a', 2, NULL),    ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemchr("",   'a', 0, &result), ==, ERROR_EMPTY_BUFFER);
    CUT_CHECK_OPERATOR_UINT32(kmemchr("as", 'd', 2, &result), ==, ERROR_NOT_FOUND);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kmemchr(
                testCases[i].str,
                testCases[i].value,
                strlen(testCases[i].str),
                &result
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_ADDRESS(
            result,
            ==,
            testCases[i].str + testCases[i].position
        );
    }
}

CUT_DEFINE_TEST(test_kmemcmp)
{
    struct TestCase {
        char str1[32];
        char str2[32];
        size_t length;
        sint32 result;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {"asd", "asd", 3,  0, ERROR_SUCCESS},
        {"asd", "ase", 3, -1, ERROR_SUCCESS},
        {"asd", "asc", 3,  1, ERROR_SUCCESS}
    };

    sint32 result = 0;

    CUT_CHECK_OPERATOR_UINT32(kmemcmp(NULL, "as", 2, &result), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemcmp("as", NULL, 2, &result), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemcmp("as", "as", 2, NULL),    ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemcmp("as", "as", 2, &result), ==, ERROR_SAME_POINTERS);
    CUT_CHECK_OPERATOR_UINT32(kmemcmp("as", "ad", 0, &result), ==, ERROR_EMPTY_BUFFER);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kmemcmp(
                testCases[i].str1,
                testCases[i].str2,
                testCases[i].length,
                &result
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_INT32(result, ==, testCases[i].result);
    }
}

CUT_DEFINE_TEST(test_kmemcpy)
{
    struct TestCase {
        char memArea1[32];
        char memArea2[32];
        size_t length;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {"asdfg", "qwert", 5, ERROR_SUCCESS},
        {"asdfg", "zxcvb", 5, ERROR_SUCCESS},
        {"asdfg", "poiuy", 5, ERROR_SUCCESS}
    };

    char memArea1[] = "asdfg";
    char memArea2[] = "qwert";

    CUT_CHECK_OPERATOR_UINT32(kmemcpy(NULL, memArea2, 5), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemcpy(memArea1, NULL, 5), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemcpy(memArea1, memArea2, 0), ==, ERROR_INSUFFICIENT_BUFFER);
    CUT_CHECK_OPERATOR_UINT32(kmemcpy(memArea1, memArea1, 5), ==, ERROR_SAME_POINTERS);

    sint32 result = 0;

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kmemcpy(
                testCases[i].memArea1,
                testCases[i].memArea2,
                testCases[i].length
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_UINT32(
            kmemcmp(
                testCases[i].memArea1,
                testCases[i].memArea2,
                testCases[i].length,
                &result
            ),
            ==,
            ERROR_SUCCESS
        );

        CUT_CHECK_OPERATOR_INT32(result, ==, 0);
    }
}

CUT_DEFINE_MODULE(module_kstring)
    CUT_CALL_TEST(test_kmemchr);
    CUT_CALL_TEST(test_kmemcmp);
    CUT_CALL_TEST(test_kmemcpy);
CUT_END_MODULE
