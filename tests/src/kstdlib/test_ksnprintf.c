#include "tests/asserts.h"

#include <cut.h>

#include "util/kstdlib/include/ksnprintf_helper.h"
#include "util/kstdlib/include/ksnprintf.h"
#include "util/kstdlib/include/kvsnprintf.h"

CUT_DEFINE_TEST(test_ksnprintf)
{

}

CUT_DEFINE_TEST(test_kvsnprintf)
{

}

CUT_DEFINE_MODULE(module_ksnprintf)
    CUT_CALL_TEST(test_ksnprintf);
    CUT_CALL_TEST(test_kvsnprintf);
CUT_END_MODULE
