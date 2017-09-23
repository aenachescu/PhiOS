#include "tests/asserts.h"

#include <cut.h>

#include "kernel/include/logging.h"

extern logging_writePfn g_pfn[];
extern uint32 g_numOfPfn;
extern bool g_isInitialized;

PhiOS_STATIC_ASSERT(sizeof(bool) == sizeof(uint8), PhiOS_ASSERT_MESSAGE);

CUT_DEFINE_TEST(test_logging_init)
{
    CUT_CHECK_OPERATOR_UINT8(g_isInitialized, ==, false);

    CUT_CHECK_OPERATOR_UINT32(logging_init(), ==, ERROR_SUCCESS);

    uint32 capacity = logging_getPfnArrayCapacity();
    for (uint32 i = 0; i < capacity; i++) {
        CUT_CHECK_OPERATOR_ADDRESS((void*)g_pfn[i], ==, (void*)NULL);
    }

    CUT_CHECK_OPERATOR_UINT32(g_numOfPfn, ==, 0);
    CUT_CHECK_OPERATOR_UINT8(g_isInitialized, ==, true);

    CUT_CHECK_OPERATOR_UINT32(logging_init(), ==, ERROR_ALREADY_INITIALIZED);

    logging_uninit();
}

CUT_DEFINE_TEST(test_logging_uninit)
{
    CUT_CHECK_OPERATOR_UINT32(logging_uninit(), ==, ERROR_UNINITIALIZED);

    logging_init();

    CUT_CHECK_OPERATOR_UINT32(logging_uninit(), ==, ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_UINT8(g_isInitialized, ==, false);
}

CUT_DEFINE_TEST(test_logging_isInitialized)
{
    CUT_CHECK_OPERATOR_UINT8(logging_isInitialized(), ==, false);

    logging_init();
    CUT_CHECK_OPERATOR_UINT8(logging_isInitialized(), ==, true);

    logging_uninit();
    CUT_CHECK_OPERATOR_UINT8(logging_isInitialized(), ==, false);
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_logging_init);
    CUT_CALL_TEST(test_logging_uninit);
    CUT_CALL_TEST(test_logging_isInitialized);
CUT_END_MAIN
