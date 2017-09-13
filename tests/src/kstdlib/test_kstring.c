#include "tests/asserts.h"

#include <cut.h>

CUT_DEFINE_TEST(test_kmemchr)
{
    CUT_CHECK(1 == 1);
}

CUT_DEFINE_MODULE(module_kstring)
    CUT_CALL_TEST(test_kmemchr);
CUT_END_MODULE
