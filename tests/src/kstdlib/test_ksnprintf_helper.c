#include "tests/asserts.h"

#include <cut.h>

CUT_DEFINE_TEST(test_p_specifier)
{

}

CUT_DEFINE_MODULE(module_ksnprintf_helper)
    CUT_CALL_TEST(test_p_specifier);
CUT_END_MODULE
