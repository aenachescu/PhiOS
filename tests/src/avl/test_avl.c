#include "tests/config.h"

#include "tests/src/avl/avl_impl.h"

#define RESET_STATISTICS    \
    ResetMemoryInUsage();   \
    ResetObjectsInUsage();  \
    ResetAllocCalls();      \
    ResetFreeCalls();

#define CHECK_STATISTICS                                                \
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);               \
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);              \
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, GetFreeCalls());

CUT_DEFINE_TEST(test_avl_init)
{
    RESET_STATISTICS;

    UTDataAVL tree;

    CUT_CHECK(UTDataAVL_init(NULL) == AVL_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_init(&tree) == AVL_ERROR_SUCCESS);

    UTDataAVL_free(&tree);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_free)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    Data data;

    CUT_CHECK(UTDataAVL_init(&tree) == AVL_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_free(NULL) == AVL_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_free(&tree) == AVL_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_init(&tree) == AVL_ERROR_SUCCESS);

    for (size_t i = 0; i < ((size_t) 1000); i++) {
        data.data = (unsigned int) i;

        CUT_CHECK(UTDataAVL_insert(&tree, &data) == AVL_ERROR_SUCCESS);

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, ((i + 1) * sizeof(UTDataAVLNode)));
    }

    CUT_CHECK(UTDataAVL_free(&tree) == AVL_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_avl_init);
    CUT_CALL_TEST(test_avl_free);
CUT_END_MAIN
