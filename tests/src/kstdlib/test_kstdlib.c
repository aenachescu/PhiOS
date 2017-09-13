#include "tests/asserts.h"

#include <cut.h>

uint32 kernel_random32()
{
    return 0;
}

CUT_DEFINE_TEST(test_kitoa)
{

}

CUT_DEFINE_MODULE(module_kstdlib)
    CUT_CALL_TEST(test_kitoa);
CUT_END_MODULE
