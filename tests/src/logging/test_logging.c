#include "tests/config.h"

#include "kernel/include/logging.h"

#include <string.h>

extern logging_writePfn g_pfn[];
extern uint32 g_numOfPfn;
extern bool g_isInitialized;

PhiOS_STATIC_ASSERT(sizeof(bool) == sizeof(uint8), PhiOS_ASSERT_MESSAGE);

char g_loggingBuffer[512] = { 0 };

uint32 logging_write(const char *str)
{
    if (str == NULL) {
        return ERROR_NULL_POINTER;
    }

    strcpy(g_loggingBuffer, str);

    return ERROR_SUCCESS;
}

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

CUT_DEFINE_TEST(test_logging_addPfn)
{
    CUT_CHECK_OPERATOR_UINT32(logging_addPfn(logging_write), ==, ERROR_UNINITIALIZED);

    logging_init();

    CUT_CHECK_OPERATOR_UINT32(logging_addPfn((logging_writePfn) NULL), ==, ERROR_INVALID_FUNCTION);

    uint32 capacity = logging_getPfnArrayCapacity();
    for (uint32 i = 0; i < capacity; i++) {
        CUT_CHECK_OPERATOR_UINT32(logging_addPfn(logging_write), ==, ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(logging_getPfnArrayLength(), ==, i + 1);
    }

    CUT_CHECK_OPERATOR_UINT32(logging_addPfn(logging_write), ==, ERROR_LIMIT_REACHED);
    CUT_CHECK_OPERATOR_UINT32(logging_getPfnArrayLength(), ==, logging_getPfnArrayCapacity());

    logging_uninit();
}

CUT_DEFINE_TEST(test_logging_write)
{
    logging_init();
    logging_addPfn(logging_write);

    KLOG_FATAL("fatal: [%s]: %u", "test1", 1);
    CUT_CHECK_OPERATOR_STRING(g_loggingBuffer, ==, "[_logging.c][   91][  FATAL]: fatal: [test1]: 1\n");

    KLOG_ERROR("error: [%s]: %u", "test2", 2);
    CUT_CHECK_OPERATOR_STRING(g_loggingBuffer, ==, "[_logging.c][   94][  ERROR]: error: [test2]: 2\n");

    KLOG_WARNING("warning: [%s]: %u", "test3", 3);
    CUT_CHECK_OPERATOR_STRING(g_loggingBuffer, ==, "[_logging.c][   97][WARNING]: warning: [test3]: 3\n");

    KLOG_INFO("info: [%s]: %u", "test4", 4);
    CUT_CHECK_OPERATOR_STRING(g_loggingBuffer, ==, "[_logging.c][  100][   INFO]: info: [test4]: 4\n");

    KLOG_TRACE("trace: [%s]: %u", "test5", 5);
    CUT_CHECK_OPERATOR_STRING(g_loggingBuffer, ==, "[_logging.c][  103][  TRACE]: trace: [test5]: 5\n");

    KLOG_DEBUG("debug: [%s]: %u", "test6", 6);
    CUT_CHECK_OPERATOR_STRING(g_loggingBuffer, ==, "[_logging.c][  106][  DEBUG]: debug: [test6]: 6\n");

    logging_uninit();
}

CUT_DEFINE_TEST(test_logging_adjustPointers)
{
    logging_writePfn *oldValue = malloc(g_numOfPfn * sizeof(logging_writePfn));
    for (uint32 i = 0 ; i < g_numOfPfn; i++) {
        oldValue[i] = g_pfn[i];
    }

    logging_adjustPointers(100);

    for (uint32 i = 0; i < g_numOfPfn; i++) {
        CUT_CHECK_OPERATOR_ADDRESS(g_pfn[i], ==, oldValue[i] + 100);
    }
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_logging_init);
    CUT_CALL_TEST(test_logging_uninit);
    CUT_CALL_TEST(test_logging_isInitialized);
    CUT_CALL_TEST(test_logging_addPfn);
    CUT_CALL_TEST(test_logging_write);

    CUT_CALL_TEST(test_logging_adjustPointers);
CUT_END_MAIN
