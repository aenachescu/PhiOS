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

CUT_DEFINE_MODULE(module_kstring)
    CUT_CALL_TEST(test_kmemchr);
CUT_END_MODULE
