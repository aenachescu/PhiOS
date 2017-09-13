#include "tests/asserts.h"

#include <cut.h>

CUT_DEFINE_TEST(test_kvsnprintf)
{

}

CUT_DEFINE_MODULE(module_kvsnprintf)
    CUT_CALL_TEST(test_kvsnprintf);
CUT_END_MODULE
