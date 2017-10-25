#include "tests/config.h"

#include "util/kstdlib/include/kstring.h"

#include <string.h>

#define _countof(x) (sizeof(x) / sizeof(x[0]))

#define MIN(a, b) (a < b ? a : b)

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

    char memArea1[] = "asdfghjk";
    char memArea2[] = "qwertyui";

    CUT_CHECK_OPERATOR_UINT32(kmemcpy(NULL, memArea2, 5), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemcpy(memArea1, NULL, 5), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemcpy(memArea1, memArea2, 0), ==, ERROR_INSUFFICIENT_BUFFER);
    CUT_CHECK_OPERATOR_UINT32(kmemcpy(memArea1, memArea1, 5), ==, ERROR_SAME_POINTERS);

    sint32 result = 0;

    CUT_CHECK_OPERATOR_UINT32(kmemcpy(memArea1, memArea1 + 3, 5), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_UINT32(kmemcmp(memArea1, "fghjkhjk", 8, &result), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_INT32(result, ==, 0);

    CUT_CHECK_OPERATOR_UINT32(kmemcpy(memArea2 + 3, memArea2, 5), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_UINT32(kmemcmp(memArea2, "qweqweqw", 8, &result), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_INT32(result, ==, 0);

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

CUT_DEFINE_TEST(test_kmemmove)
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

    char memArea1[] = "asdfghjk";
    char memArea2[] = "qwertyui";

    CUT_CHECK_OPERATOR_UINT32(kmemmove(NULL, memArea2, 5), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemmove(memArea1, NULL, 5), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kmemmove(memArea1, memArea2, 0), ==, ERROR_EMPTY_BUFFER);
    CUT_CHECK_OPERATOR_UINT32(kmemmove(memArea1, memArea1, 5), ==, ERROR_SAME_POINTERS);

    sint32 result = 0;

    CUT_CHECK_OPERATOR_UINT32(kmemmove(memArea1, memArea1 + 3, 5), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_UINT32(kmemcmp(memArea1, "fghjkhjk", 8, &result), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_INT32(result, ==, 0);

    CUT_CHECK_OPERATOR_UINT32(kmemmove(memArea2 + 3, memArea2, 5), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_UINT32(kmemcmp(memArea2, "qweqwert", 8, &result), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_INT32(result, ==, 0);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kmemmove(
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

CUT_DEFINE_TEST(test_kmemset)
{
    struct TestCase {
        char buffer[32];
        char result[32];
        size_t length;
        sint8 value;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {"qwert", "11111", 5, '1', ERROR_SUCCESS},
        {"asdfg", "22222", 5, '2', ERROR_SUCCESS},
        {"zxc"  , "333"  , 3, '3', ERROR_SUCCESS}
    };

    char memArea[] = "asdfg";
    sint32 result;

    CUT_CHECK_OPERATOR_UINT32(kmemset(memArea, '1', 0), ==, ERROR_EMPTY_BUFFER);
    CUT_CHECK_OPERATOR_UINT32(kmemset(NULL, '1', 5), ==, ERROR_NULL_POINTER);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kmemset(
                testCases[i].buffer,
                testCases[i].value,
                testCases[i].length
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_UINT32(
            kmemcmp(
                testCases[i].buffer,
                testCases[i].result,
                testCases[i].length + 1,
                &result
            ),
            ==,
            ERROR_SUCCESS
        );

        CUT_CHECK_OPERATOR_INT32(result, ==, 0);
    }
}

CUT_DEFINE_TEST(test_kstrcmp)
{
    struct TestCase {
        char str1[32];
        char str2[32];
        sint32 result;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {"asdasd", "asdasd",  0, ERROR_SUCCESS},
        {"asd"   , "asd"   ,  0, ERROR_SUCCESS},
        {"asdasd", "asdasf", -2, ERROR_SUCCESS},
        {"asd"   , "asf"   , -2, ERROR_SUCCESS},
        {"asdasf", "asdasd",  2, ERROR_SUCCESS},
        {"asf"   , "asd"   ,  2, ERROR_SUCCESS},
        {"asdasd", "asf"   , -2, ERROR_SUCCESS},
        {"asdasd", "asfasf", -2, ERROR_SUCCESS},
        {"asf"   , "asdasd",  2, ERROR_SUCCESS},
        {"asfasf", "asdasd",  2, ERROR_SUCCESS},
        {"asdf"  , "asd"   ,  1, ERROR_SUCCESS},
        {"asd"   , "asdf"  , -1, ERROR_SUCCESS},
    };

    char str1[] = "asdfg";
    char str2[] = "zxcvb";
    sint32 result;

    CUT_CHECK_OPERATOR_UINT32(kstrcmp(NULL, str2, &result), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kstrcmp(str1, NULL, &result), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kstrcmp(str1, str1, &result), ==, ERROR_SAME_POINTERS);
    CUT_CHECK_OPERATOR_UINT32(kstrcmp(str1, str2, NULL)   , ==, ERROR_NULL_POINTER);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kstrcmp(
                testCases[i].str1,
                testCases[i].str2,
                &result
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_INT32(result, ==, testCases[i].result);
    }
}

CUT_DEFINE_TEST(test_kstrcpy)
{
    struct TestCase {
        char str1[32];
        char str2[32];
        size_t length;
        size_t expectedLength;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {"asdfg", "qwe"  , 6, 4, ERROR_SUCCESS},
        {"asd"  , "asdfg", 4, 6, ERROR_INSUFFICIENT_BUFFER},
        {"a"    , "fg"   , 2, 3, ERROR_INSUFFICIENT_BUFFER},
        {"asd"  , "b"    , 4, 2, ERROR_SUCCESS},
        {"a"    , "b"    , 2, 2, ERROR_SUCCESS},
    };

    char str1[] = "asdfg";
    char str2[] = "zxcvb";
    size_t length;
    uint32 retValue;
    sint32 result;

    CUT_CHECK_OPERATOR_UINT32(kstrcpy(str1, &length, NULL), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kstrcpy(NULL, &length, str2), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kstrcpy(str1, NULL, str2), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kstrcpy(str1, &length, str1), ==, ERROR_SAME_POINTERS);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        length = testCases[i].length;
        retValue = kstrcpy(testCases[i].str1, &length, testCases[i].str2);
        CUT_CHECK_OPERATOR_UINT32(retValue, ==, testCases[i].retValue);

        CUT_CHECK_OPERATOR_SIZE_T(length, ==, testCases[i].expectedLength);

        CUT_CHECK_OPERATOR_UINT32(
            kmemcmp(
                testCases[i].str1,
                testCases[i].str2,
                MIN(testCases[i].length, testCases[i].expectedLength),
                &result
            ),
            ==,
            ERROR_SUCCESS
        );

        CUT_CHECK_OPERATOR_INT32(result, ==, 0);
    }
}

CUT_DEFINE_TEST(test_kstrlen)
{
    struct TestCase {
        char str[32];
        size_t length;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {""     , 0, ERROR_SUCCESS},
        {"a"    , 1, ERROR_SUCCESS},
        {"as"   , 2, ERROR_SUCCESS},
        {"asd"  , 3, ERROR_SUCCESS},
        {"asdf" , 4, ERROR_SUCCESS}
    };

    char str[] = "asdfg";
    size_t length;

    CUT_CHECK_OPERATOR_UINT32(kstrlen(NULL, &length), ==, ERROR_NULL_POINTER);
    CUT_CHECK_OPERATOR_UINT32(kstrlen(str, NULL), ==, ERROR_NULL_POINTER);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kstrlen(
                testCases[i].str,
                &length
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_SIZE_T(length, ==, testCases[i].length);
    }
}

CUT_DEFINE_TEST(test_kstrrev)
{
    struct TestCase {
        char str1[32];
        char str2[32];
        size_t length;
        uint32 retValue;
    };

    static struct TestCase testCases[] = {
        {""   , ""   , 0, ERROR_SUCCESS},
        {"asd", "dsa", 3, ERROR_SUCCESS},
        {"aaa", "aaa", 3, ERROR_SUCCESS},
        {"asa", "asa", 3, ERROR_SUCCESS},
        {"qwe", "ewq", 0, ERROR_SUCCESS},
    };

    sint32 result;

    CUT_CHECK_OPERATOR_UINT32(kstrrev(NULL, 0), ==, ERROR_NULL_POINTER);

    for (uint32 i = 0; i < _countof(testCases); i++) {
        CUT_CHECK_OPERATOR_UINT32(
            kstrrev(
                testCases[i].str1,
                testCases[i].length
            ),
            ==,
            testCases[i].retValue
        );

        CUT_CHECK_OPERATOR_UINT32(
            kstrcmp(
                testCases[i].str1,
                testCases[i].str2,
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
    CUT_CALL_TEST(test_kmemmove);
    CUT_CALL_TEST(test_kmemset);
    CUT_CALL_TEST(test_kstrcmp);
    CUT_CALL_TEST(test_kstrcpy);
    CUT_CALL_TEST(test_kstrlen);
    CUT_CALL_TEST(test_kstrrev);
CUT_END_MODULE
