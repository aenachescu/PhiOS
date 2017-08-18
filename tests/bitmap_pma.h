#include <cut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CUT_DEFINE_TEST(test1)
{
    CUT_CHECK(strcmp("string", "string") == 0);
    CUT_CHECK(strcmp("a", "b") == 0);
}