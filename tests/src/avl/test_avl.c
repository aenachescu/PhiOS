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

#define _countof(x) (sizeof(x) / sizeof(x[0]))

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

CUT_DEFINE_TEST(test_avl_node_init)
{
    RESET_STATISTICS;

    Data data;
    UTDataAVLNode node;

    data.data = 5;

    CUT_CHECK(UTDataAVLNode_init(NULL, &data) == AVL_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVLNode_init(&node, NULL) == AVL_ERROR_NULL_POINTER);

    CUT_CHECK(UTDataAVLNode_init(&node, &data) == AVL_ERROR_SUCCESS);

    CUT_CHECK(node.left == NULL);
    CUT_CHECK(node.right == NULL);
    CUT_CHECK(node.height == 1);
    CUT_CHECK(node.data.data == 5);

    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    CUT_CHECK(UTDataAVLNode_uninit(&node) == AVL_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_node_uninit)
{
    RESET_STATISTICS;

    UTDataAVLNode node, left, right;
    Data data;

    CUT_CHECK(UTDataAVLNode_init(&node, &data) == AVL_ERROR_SUCCESS);

    data.data = 10;
    node.left = &left;
    node.right = &right;

    CUT_CHECK(UTDataAVLNode_uninit(NULL) == AVL_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVLNode_uninit(&node) == AVL_ERROR_SUCCESS);

    CUT_CHECK(node.left == NULL);
    CUT_CHECK(node.right == NULL);
    CUT_CHECK(node.height == 0);
    CUT_CHECK(node.data.data == 0);

    CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, 0);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_node_create)
{
    RESET_STATISTICS;

    Data data;
    UTDataAVLNode *node = NULL;

    data.data = 15;

    CUT_CHECK(UTDataAVLNode_create(NULL, &data) == AVL_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVLNode_create(&node, NULL) == AVL_ERROR_NULL_POINTER);

    CUT_CHECK(UTDataAVLNode_create(&node, &data) == AVL_ERROR_SUCCESS);

    CUT_CHECK(node != NULL);
    CUT_CHECK(node->left == NULL);
    CUT_CHECK(node->right == NULL);
    CUT_CHECK(node->height == 1);
    CUT_CHECK(node->data.data == 15);

    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, sizeof(UTDataAVLNode));

    CUT_CHECK(UTDataAVLNode_free(node) == AVL_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_node_free)
{
    RESET_STATISTICS;

    Data data1, data2, data3;
    UTDataAVLNode *node = NULL, *left = NULL, *right = NULL;

    data1.data = 20;
    data2.data = 30;
    data3.data = 40;

    CUT_CHECK(UTDataAVLNode_create(&node, &data1) == AVL_ERROR_SUCCESS);
    CUT_ASSERT(node != NULL);

    CUT_CHECK(UTDataAVLNode_free(NULL) == AVL_ERROR_NULL_POINTER);

    CUT_CHECK(UTDataAVLNode_free(node) == AVL_ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    CUT_CHECK(UTDataAVLNode_create(&node, &data1) == AVL_ERROR_SUCCESS);
    CUT_ASSERT(node != NULL);
    CUT_CHECK(UTDataAVLNode_create(&left, &data2) == AVL_ERROR_SUCCESS);
    CUT_ASSERT(left != NULL);
    CUT_CHECK(UTDataAVLNode_create(&right, &data3) == AVL_ERROR_SUCCESS);
    CUT_ASSERT(right != NULL);

    node->left = left;
    node->right = right;

    CUT_CHECK(UTDataAVLNode_free(node) == AVL_ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, 4);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_getHeight)
{
    RESET_STATISTICS;

    static struct TestCases {
        Data data;
        unsigned int expectedHeight;
    } testCases[] = {
        { { 61 }, 1 },
        { { 58 }, 2 },
        { { 41 }, 2 },
        { {  5 }, 3 },
        { { 29 }, 3 },
        { { 17 }, 3 },
        { { 25 }, 3 },
        { { 97 }, 4 },
        { { 87 }, 4 },
        { { 57 }, 4 },
    };

    UTDataAVL tree;
    unsigned int height;

    CUT_ASSERT(UTDataAVL_init(&tree) == AVL_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_getHeight(NULL, &height) == AVL_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_getHeight(&tree, NULL) == AVL_ERROR_NULL_POINTER);

    CUT_CHECK(UTDataAVL_getHeight(&tree, &height) == AVL_ERROR_SUCCESS);
    CUT_CHECK(height == 0);

    for (size_t i = 0; i < _countof(testCases); i++) {
        CUT_CHECK(UTDataAVL_insert(&tree, &testCases[i].data) == AVL_ERROR_SUCCESS);
        CUT_CHECK(UTDataAVL_getHeight(&tree, &height) == AVL_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(height, ==, testCases[i].expectedHeight);
    }

    CUT_CHECK(UTDataAVL_free(&tree) == AVL_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_MAIN
    CUT_CALL_TEST(test_avl_init);
    CUT_CALL_TEST(test_avl_free);

    CUT_CALL_TEST(test_avl_node_init);
    CUT_CALL_TEST(test_avl_node_uninit);
    CUT_CALL_TEST(test_avl_node_create);
    CUT_CALL_TEST(test_avl_node_free);

    CUT_CALL_TEST(test_avl_getHeight);

CUT_END_MAIN
