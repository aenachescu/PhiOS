#include "tests/config.h"

#include "tests/src/avl/avl_impl.h"

CUT_DEFINE_TEST(test_avl_init)
{
    CUT_CHECK_OPERATOR_UINT32(0, ==, 0);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_avl_init);
CUT_END_MAIN
