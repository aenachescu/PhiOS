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

    CUT_CHECK(UTDataAVL_init(NULL) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(tree.root == NULL);

    UTDataAVL_free(&tree);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_free)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    Data data;

    CUT_CHECK(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_free(NULL) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    for (size_t i = 0; i < ((size_t) 1000); i++) {
        data.data = (unsigned int) i;

        CUT_CHECK(UTDataAVL_insert(&tree, &data) == CLIB_ERROR_SUCCESS);

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, ((i + 1) * sizeof(UTDataAVLNode)));
    }

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_ADDRESS(tree.root, ==, NULL);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_node_init)
{
    RESET_STATISTICS;

    Data data;
    UTDataAVLNode node;

    data.data = 5;

    CUT_CHECK(UTDataAVLNode_init(NULL, &data) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVLNode_init(&node, NULL) == CLIB_ERROR_NULL_POINTER);

    AVLCopyDataSetError();
    CUT_CHECK(UTDataAVLNode_init(&node, &data) == CLIB_ERROR_INTERNAL_ERROR);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    CUT_CHECK(UTDataAVLNode_init(&node, &data) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(node.left == NULL);
    CUT_CHECK(node.right == NULL);
    CUT_CHECK(node.height == 1);
    CUT_CHECK(node.data.data == 5);

    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    CUT_CHECK(UTDataAVLNode_uninit(&node) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_node_uninit)
{
    RESET_STATISTICS;

    UTDataAVLNode node, left, right;
    Data data;

    CUT_CHECK(UTDataAVLNode_init(&node, &data) == CLIB_ERROR_SUCCESS);

    data.data = 10;
    node.left = &left;
    node.right = &right;

    CUT_CHECK(UTDataAVLNode_uninit(NULL) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVLNode_uninit(&node) == CLIB_ERROR_SUCCESS);

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

    CUT_CHECK(UTDataAVLNode_create(NULL, &data) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVLNode_create(&node, NULL) == CLIB_ERROR_NULL_POINTER);

    AVLAllocNodeSetError();
    CUT_CHECK(UTDataAVLNode_create(&node, &data) == CLIB_ERROR_NO_FREE_MEMORY);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    AVLCopyDataSetError();
    CUT_CHECK(UTDataAVLNode_create(&node, &data) == CLIB_ERROR_INTERNAL_ERROR);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    CUT_CHECK(UTDataAVLNode_create(&node, &data) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(node != NULL);
    CUT_CHECK(node->left == NULL);
    CUT_CHECK(node->right == NULL);
    CUT_CHECK(node->height == 1);
    CUT_CHECK(node->data.data == 15);

    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 2);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, sizeof(UTDataAVLNode));

    CUT_CHECK(UTDataAVLNode_free(node) == CLIB_ERROR_SUCCESS);

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

    CUT_CHECK(UTDataAVLNode_create(&node, &data1) == CLIB_ERROR_SUCCESS);
    CUT_ASSERT(node != NULL);

    CUT_CHECK(UTDataAVLNode_free(NULL) == CLIB_ERROR_NULL_POINTER);

    CUT_CHECK(UTDataAVLNode_free(node) == CLIB_ERROR_SUCCESS);
    CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    CUT_CHECK(UTDataAVLNode_create(&node, &data1) == CLIB_ERROR_SUCCESS);
    CUT_ASSERT(node != NULL);
    CUT_CHECK(UTDataAVLNode_create(&left, &data2) == CLIB_ERROR_SUCCESS);
    CUT_ASSERT(left != NULL);
    CUT_CHECK(UTDataAVLNode_create(&right, &data3) == CLIB_ERROR_SUCCESS);
    CUT_ASSERT(right != NULL);

    node->left = left;
    node->right = right;

    CUT_CHECK(UTDataAVLNode_free(node) == CLIB_ERROR_SUCCESS);
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

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_getHeight(NULL, &height) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_getHeight(&tree, NULL) == CLIB_ERROR_NULL_POINTER);

    CUT_CHECK(UTDataAVL_getHeight(&tree, &height) == CLIB_ERROR_SUCCESS);
    CUT_CHECK(height == 0);

    for (size_t i = 0; i < _countof(testCases); i++) {
        CUT_CHECK(UTDataAVL_insert(&tree, &testCases[i].data) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(UTDataAVL_getHeight(&tree, &height) == CLIB_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(height, ==, testCases[i].expectedHeight);
    }

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

struct AvlInsertTestCase
{
    Data data;
    unsigned int expectedHeight;
};

static struct AvlInsertTestCase g_insertTestCases[] = {
    { {  125 }, 1 }, { {  473 }, 2 }, { {  213 }, 2 }, { {  563 }, 3 }, { {  477 }, 3 },
    { {  910 }, 3 }, { {   22 }, 4 }, { {  159 }, 4 }, { {  259 }, 4 }, { {  371 }, 4 },
    { {  274 }, 4 }, { {  250 }, 4 }, { {  281 }, 5 }, { {   19 }, 5 }, { {  841 }, 5 },
    { {  153 }, 5 }, { {  699 }, 5 }, { {  939 }, 5 }, { {   72 }, 5 }, { {  920 }, 5 },
    { {  744 }, 5 }, { {  654 }, 5 }, { {  346 }, 5 }, { {  705 }, 5 }, { {   94 }, 6 },
    { {  573 }, 6 }, { {   33 }, 6 }, { {  190 }, 6 }, { {  886 }, 6 }, { {  472 }, 6 },
    { {  494 }, 6 }, { {  410 }, 6 }, { {  717 }, 6 }, { {  532 }, 6 }, { {  954 }, 6 },
    { {  883 }, 6 }, { {  838 }, 6 }, { {  847 }, 7 }, { {  553 }, 7 }, { {   30 }, 7 },
    { {  190 }, 7 }, { {  248 }, 7 }, { {  426 }, 7 }, { {  216 }, 7 }, { {  721 }, 7 },
    { {  196 }, 7 }, { {  365 }, 7 }, { {  548 }, 7 }, { {   85 }, 7 }, { {  748 }, 7 },
    { {  737 }, 7 }, { {  864 }, 7 }, { {  987 }, 7 }, { {  886 }, 7 }, { {  948 }, 7 },
    { {   70 }, 7 }, { {  433 }, 7 }, { {  975 }, 7 }, { {   76 }, 7 }, { {  759 }, 7 },
    { {  320 }, 7 }, { {  892 }, 7 }, { {  213 }, 7 }, { {   79 }, 7 }, { {  819 }, 7 },
    { {  829 }, 7 }, { {  899 }, 7 }, { {  999 }, 7 }, { {  992 }, 7 }, { {  405 }, 8 },
    { {  683 }, 8 }, { {  594 }, 8 }, { {  148 }, 8 }, { {  122 }, 8 }, { {   61 }, 8 },
    { {  699 }, 8 }, { {  499 }, 8 }, { {   42 }, 8 }, { {  231 }, 8 }, { {  215 }, 8 },
    { {  475 }, 8 }, { {  539 }, 8 }, { {  100 }, 8 }, { {  291 }, 8 }, { {  153 }, 8 },
    { {  128 }, 8 }, { {  572 }, 8 }, { {  497 }, 8 }, { {  623 }, 8 }, { {  449 }, 8 },
    { {  553 }, 8 }, { {  944 }, 8 }, { {  456 }, 8 }, { {  553 }, 8 }, { {  935 }, 8 },
    { {  199 }, 8 }, { {  756 }, 8 }, { {    7 }, 8 }, { {  184 }, 8 }, { {  617 }, 8 },
    { {  380 }, 8 }, { {  799 }, 8 }, { {  497 }, 8 }, { {  962 }, 8 }, { {  120 }, 8 },
    { {  746 }, 8 }, { {  121 }, 8 }, { {  798 }, 8 }, { {  395 }, 8 }, { {  154 }, 8 },
    { {  201 }, 8 }, { {  171 }, 8 }, { {  601 }, 8 }, { {  188 }, 8 }, { {  926 }, 8 },
    { {  609 }, 8 }, { {  870 }, 8 }, { {  716 }, 8 }, { {  902 }, 8 }, { {  901 }, 8 },
    { {  809 }, 8 }, { {  483 }, 8 }, { {  809 }, 8 }, { {  969 }, 8 }, { {  495 }, 8 },
    { {    9 }, 8 }, { {   13 }, 8 }, { {  704 }, 8 }, { {  476 }, 8 }, { {  189 }, 8 },
    { {  594 }, 8 }, { {  162 }, 8 }, { {  951 }, 8 }, { {   36 }, 8 }, { {  738 }, 8 },
    { {  747 }, 8 }, { {   15 }, 8 }, { {  717 }, 8 }, { {  659 }, 8 }, { {  648 }, 8 },
    { {  341 }, 8 }, { {  981 }, 9 }, { {  394 }, 9 }, { {  150 }, 9 }, { {  740 }, 9 },
    { {  261 }, 9 }, { {  654 }, 9 }, { {  963 }, 9 }, { {  355 }, 9 }, { {   91 }, 9 },
    { {  933 }, 9 }, { {  316 }, 9 }, { {  308 }, 9 }, { {  461 }, 9 }, { {  228 }, 9 },
    { {  354 }, 9 }, { {  941 }, 9 }, { {  618 }, 9 }, { {  554 }, 9 }, { {  796 }, 9 },
    { {  638 }, 9 }, { {  502 }, 9 }, { {  868 }, 9 }, { {  168 }, 9 }, { {  692 }, 9 },
    { {  420 }, 9 }, { {  870 }, 9 }, { {  783 }, 9 }, { {  454 }, 9 }, { {  790 }, 9 },
    { {  455 }, 9 }, { {  909 }, 9 }, { {  272 }, 9 }, { {  575 }, 9 }, { {   46 }, 9 },
    { {  544 }, 9 }, { {  998 }, 9 }, { {  412 }, 9 }, { {   57 }, 9 }, { {  388 }, 9 },
    { {  132 }, 9 }, { {  249 }, 9 }, { {  549 }, 9 }, { {   84 }, 9 }, { {  394 }, 9 },
    { {  459 }, 9 }, { {  548 }, 9 }, { {  156 }, 9 }, { {  870 }, 9 }, { {  774 }, 9 },
    { {  939 }, 9 }, { {  108 }, 9 }, { {  663 }, 9 }, { {  832 }, 9 }, { {  891 }, 9 },
    { {  216 }, 9 }, { {  227 }, 9 }, { {  616 }, 9 }, { {    1 }, 9 }, { {  260 }, 9 },
};

static char g_avlTraversalsBuffer[2048];

int DataSortCmp(const void *a, const void *b)
{
	return ( (*((unsigned int*)a))  - (*((unsigned int*)b)) );
}

void getSubarraySortedAsString(char *buffer, size_t pos)
{
	unsigned int *vec = (unsigned int*)malloc(sizeof(unsigned int) * (pos + 1));
	if (vec == NULL) {
		return;
	}

	for (size_t i = 0; i <= pos; i++) {
		vec[i] = g_insertTestCases[i].data.data;
	}

	qsort(vec, pos + 1, sizeof(unsigned int), DataSortCmp);

	buffer[0] = 0;

	char numBuf[64];
	for (size_t i = 0; i <= pos; i++) {
		snprintf(numBuf, 64, "%2u ", vec[i]);
		strcat(buffer, numBuf);
	}

	free(vec);
}

void DataToString(const Data *data)
{
    char buf[64];
    snprintf(buf, 64, "%2u ", data->data);
    strcat(g_avlTraversalsBuffer, buf);
}

CUT_DEFINE_TEST(test_avl_insert)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    unsigned int height;
    const Data *data = NULL;
    char buffer[2048];

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_insert(&tree, NULL) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_insert(NULL, &g_insertTestCases[0].data) == CLIB_ERROR_NULL_POINTER);

    AVLAllocNodeSetError();
    CUT_CHECK(UTDataAVL_insert(&tree, &g_insertTestCases[0].data) == CLIB_ERROR_NO_FREE_MEMORY);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    for (size_t i = 0; i < _countof(g_insertTestCases); i++) {
        CUT_CHECK(UTDataAVL_insert(&tree, &g_insertTestCases[i].data) == CLIB_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, (i + 1) * sizeof(UTDataAVLNode));

        CUT_CHECK(UTDataAVL_getHeight(&tree, &height) == CLIB_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(height, ==, g_insertTestCases[i].expectedHeight);

        CUT_CHECK(UTDataAVL_findType(&tree, &g_insertTestCases[i].data, &data) == CLIB_ERROR_SUCCESS);

        getSubarraySortedAsString(buffer, i);
        g_avlTraversalsBuffer[0] = 0;

        CUT_CHECK(UTDataAVL_foreachInorder(&tree, DataToString) == CLIB_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_STRING(g_avlTraversalsBuffer, ==, buffer);
    }

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_insertNode)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    unsigned int height;
    const Data *data = NULL;
    UTDataAVLNode *node = NULL;
    UTDataAVLNode unusedNode;
    char buffer[2048];

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_insertNode(&tree, NULL) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_insertNode(NULL, &unusedNode) == CLIB_ERROR_NULL_POINTER);

    for (size_t i = 0; i < _countof(g_insertTestCases); i++) {
        CUT_CHECK(UTDataAVLNode_create(&node, &g_insertTestCases[i].data) == CLIB_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, (i + 1) * sizeof(UTDataAVLNode));

        CUT_CHECK(UTDataAVL_insertNode(&tree, node) == CLIB_ERROR_SUCCESS);

        CUT_CHECK(UTDataAVL_getHeight(&tree, &height) == CLIB_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(height, ==, g_insertTestCases[i].expectedHeight);

        CUT_CHECK(UTDataAVL_findType(&tree, &g_insertTestCases[i].data, &data) == CLIB_ERROR_SUCCESS);

        getSubarraySortedAsString(buffer, i);
        g_avlTraversalsBuffer[0] = 0;

        CUT_CHECK(UTDataAVL_foreachInorder(&tree, DataToString) == CLIB_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_STRING(g_avlTraversalsBuffer, ==, buffer);
    }

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_inorder)
{
    RESET_STATISTICS;

    static struct AvlInorderTestCase {
        Data data;
        const char *expectedString;
    } testCases[] = {
        { { 70 }, "70 " },
        { { 75 }, "70 75 " },
        { { 32 }, "32 70 75 " },
        { { 12 }, "12 32 70 75 " },
        { { 79 }, "12 32 70 75 79 " },
        { { 55 }, "12 32 55 70 75 79 " },
        { { 98 }, "12 32 55 70 75 79 98 " },
        { { 42 }, "12 32 42 55 70 75 79 98 " },
        { {  5 }, " 5 12 32 42 55 70 75 79 98 " },
        { { 38 }, " 5 12 32 38 42 55 70 75 79 98 " },
        { { 60 }, " 5 12 32 38 42 55 60 70 75 79 98 " },
        { { 66 }, " 5 12 32 38 42 55 60 66 70 75 79 98 " },
        { { 69 }, " 5 12 32 38 42 55 60 66 69 70 75 79 98 " },
        { { 36 }, " 5 12 32 36 38 42 55 60 66 69 70 75 79 98 " },
        { { 87 }, " 5 12 32 36 38 42 55 60 66 69 70 75 79 87 98 " },
        { { 14 }, " 5 12 14 32 36 38 42 55 60 66 69 70 75 79 87 98 " },
        { { 23 }, " 5 12 14 23 32 36 38 42 55 60 66 69 70 75 79 87 98 " },
        { { 95 }, " 5 12 14 23 32 36 38 42 55 60 66 69 70 75 79 87 95 98 " },
        { { 96 }, " 5 12 14 23 32 36 38 42 55 60 66 69 70 75 79 87 95 96 98 " },
        { { 52 }, " 5 12 14 23 32 36 38 42 52 55 60 66 69 70 75 79 87 95 96 98 " },
        { { 22 }, " 5 12 14 22 23 32 36 38 42 52 55 60 66 69 70 75 79 87 95 96 98 " },
        { { 21 }, " 5 12 14 21 22 23 32 36 38 42 52 55 60 66 69 70 75 79 87 95 96 98 " },
        { { 59 }, " 5 12 14 21 22 23 32 36 38 42 52 55 59 60 66 69 70 75 79 87 95 96 98 " },
        { { 82 }, " 5 12 14 21 22 23 32 36 38 42 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 19 }, " 5 12 14 19 21 22 23 32 36 38 42 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 10 }, " 5 10 12 14 19 21 22 23 32 36 38 42 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 45 }, " 5 10 12 14 19 21 22 23 32 36 38 42 45 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 46 }, " 5 10 12 14 19 21 22 23 32 36 38 42 45 46 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 92 }, " 5 10 12 14 19 21 22 23 32 36 38 42 45 46 52 55 59 60 66 69 70 75 79 82 87 92 95 96 98 " },
        { { 20 }, " 5 10 12 14 19 20 21 22 23 32 36 38 42 45 46 52 55 59 60 66 69 70 75 79 82 87 92 95 96 98 " },
        { { 31 }, " 5 10 12 14 19 20 21 22 23 31 32 36 38 42 45 46 52 55 59 60 66 69 70 75 79 82 87 92 95 96 98 " },
        { { 43 }, " 5 10 12 14 19 20 21 22 23 31 32 36 38 42 43 45 46 52 55 59 60 66 69 70 75 79 82 87 92 95 96 98 " },
    };

    UTDataAVL tree;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_foreachInorder(&tree, NULL) == CLIB_ERROR_INVALID_FUNCTION);
    CUT_CHECK(UTDataAVL_foreachInorder(NULL, DataToString) == CLIB_ERROR_NULL_POINTER);

    for (size_t i = 0; i < _countof(testCases); i++) {
        CUT_CHECK(UTDataAVL_insert(&tree, &testCases[i].data) == CLIB_ERROR_SUCCESS);

        g_avlTraversalsBuffer[0] = 0;
        CUT_CHECK(UTDataAVL_foreachInorder(&tree, DataToString) == CLIB_ERROR_SUCCESS);

        CUT_CHECK_OPERATOR_STRING(g_avlTraversalsBuffer, ==, testCases[i].expectedString);

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
    }

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_postorder)
{
    RESET_STATISTICS;

    static struct AvlPostorderTestCase {
        Data data;
        const char *expectedString;
    } testCases[] = {
        { { 70 }, "70 " },
        { { 75 }, "75 70 " },
        { { 32 }, "32 75 70 " },
        { { 12 }, "12 32 75 70 " },
        { { 79 }, "12 32 79 75 70 " },
        { { 55 }, "12 55 32 79 75 70 " },
        { { 98 }, "12 55 32 75 98 79 70 " },
        { { 42 }, "12 42 55 32 75 98 79 70 " },
        { {  5 }, " 5 12 42 55 32 75 98 79 70 " },
        { { 38 }, " 5 12 38 55 42 32 75 98 79 70 " },
        { { 60 }, " 5 12 38 32 60 55 75 98 79 70 42 " },
        { { 66 }, " 5 12 38 32 55 66 60 75 98 79 70 42 " },
        { { 69 }, " 5 12 38 32 55 69 66 60 75 98 79 70 42 " },
        { { 36 }, " 5 12 36 38 32 55 69 66 60 75 98 79 70 42 " },
        { { 87 }, " 5 12 36 38 32 55 69 66 60 75 87 98 79 70 42 " },
        { { 14 }, " 5 14 12 36 38 32 55 69 66 60 75 87 98 79 70 42 " },
        { { 23 }, " 5 23 14 12 36 38 32 55 69 66 60 75 87 98 79 70 42 " },
        { { 95 }, " 5 23 14 12 36 38 32 55 69 66 60 75 87 98 95 79 70 42 " },
        { { 96 }, " 5 23 14 12 36 38 32 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 52 }, " 5 23 14 12 36 38 32 52 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 22 }, " 5 14 23 22 12 36 38 32 52 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 21 }, " 5 12 21 23 22 14 36 38 32 52 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 59 }, " 5 12 21 23 22 14 36 38 32 52 59 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 82 }, " 5 12 21 23 22 14 36 38 32 52 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 19 }, " 5 12 19 21 14 23 36 38 32 22 52 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 10 }, " 5 12 10 19 21 14 23 36 38 32 22 52 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 45 }, " 5 12 10 19 21 14 23 36 38 32 22 45 52 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 46 }, " 5 12 10 19 21 14 23 36 38 32 22 45 52 46 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 92 }, " 5 12 10 19 21 14 23 36 38 32 22 45 52 46 59 55 69 66 60 75 82 92 87 79 96 98 95 70 42 " },
        { { 20 }, " 5 12 10 19 21 20 14 23 36 38 32 22 45 52 46 59 55 69 66 60 75 82 92 87 79 96 98 95 70 42 " },
        { { 31 }, " 5 12 10 19 21 20 14 31 23 36 38 32 22 45 52 46 59 55 69 66 60 75 82 92 87 79 96 98 95 70 42 " },
        { { 43 }, " 5 12 10 19 21 20 14 31 23 36 38 32 22 43 45 52 59 55 46 69 66 60 75 82 92 87 79 96 98 95 70 42 " },
    };

    UTDataAVL tree;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_foreachPostorder(&tree, NULL) == CLIB_ERROR_INVALID_FUNCTION);
    CUT_CHECK(UTDataAVL_foreachPostorder(NULL, DataToString) == CLIB_ERROR_NULL_POINTER);

    for (size_t i = 0; i < _countof(testCases); i++) {
        CUT_CHECK(UTDataAVL_insert(&tree, &testCases[i].data) == CLIB_ERROR_SUCCESS);

        g_avlTraversalsBuffer[0] = 0;
        CUT_CHECK(UTDataAVL_foreachPostorder(&tree, DataToString) == CLIB_ERROR_SUCCESS);

        CUT_CHECK_OPERATOR_STRING(g_avlTraversalsBuffer, ==, testCases[i].expectedString);

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
    }

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_preorder)
{
    RESET_STATISTICS;

    static struct AvlPreorderTestCase {
        Data data;
        const char *expectedString;
    } testCases[] = {
        { { 70 }, "70 " },
        { { 75 }, "70 75 " },
        { { 32 }, "70 32 75 " },
        { { 12 }, "70 32 12 75 " },
        { { 79 }, "70 32 12 75 79 " },
        { { 55 }, "70 32 12 55 75 79 " },
        { { 98 }, "70 32 12 55 79 75 98 " },
        { { 42 }, "70 32 12 55 42 79 75 98 " },
        { {  5 }, "70 32 12  5 55 42 79 75 98 " },
        { { 38 }, "70 32 12  5 42 38 55 79 75 98 " },
        { { 60 }, "42 32 12  5 38 70 55 60 79 75 98 " },
        { { 66 }, "42 32 12  5 38 70 60 55 66 79 75 98 " },
        { { 69 }, "42 32 12  5 38 70 60 55 66 69 79 75 98 " },
        { { 36 }, "42 32 12  5 38 36 70 60 55 66 69 79 75 98 " },
        { { 87 }, "42 32 12  5 38 36 70 60 55 66 69 79 75 98 87 " },
        { { 14 }, "42 32 12  5 14 38 36 70 60 55 66 69 79 75 98 87 " },
        { { 23 }, "42 32 12  5 14 23 38 36 70 60 55 66 69 79 75 98 87 " },
        { { 95 }, "42 32 12  5 14 23 38 36 70 60 55 66 69 79 75 95 87 98 " },
        { { 96 }, "42 32 12  5 14 23 38 36 70 60 55 66 69 95 79 75 87 98 96 " },
        { { 52 }, "42 32 12  5 14 23 38 36 70 60 55 52 66 69 95 79 75 87 98 96 " },
        { { 22 }, "42 32 12  5 22 14 23 38 36 70 60 55 52 66 69 95 79 75 87 98 96 " },
        { { 21 }, "42 32 14 12  5 22 21 23 38 36 70 60 55 52 66 69 95 79 75 87 98 96 " },
        { { 59 }, "42 32 14 12  5 22 21 23 38 36 70 60 55 52 59 66 69 95 79 75 87 98 96 " },
        { { 82 }, "42 32 14 12  5 22 21 23 38 36 70 60 55 52 59 66 69 95 79 75 87 82 98 96 " },
        { { 19 }, "42 22 14 12  5 21 19 32 23 38 36 70 60 55 52 59 66 69 95 79 75 87 82 98 96 " },
        { { 10 }, "42 22 14 10  5 12 21 19 32 23 38 36 70 60 55 52 59 66 69 95 79 75 87 82 98 96 " },
        { { 45 }, "42 22 14 10  5 12 21 19 32 23 38 36 70 60 55 52 45 59 66 69 95 79 75 87 82 98 96 " },
        { { 46 }, "42 22 14 10  5 12 21 19 32 23 38 36 70 60 55 46 45 52 59 66 69 95 79 75 87 82 98 96 " },
        { { 92 }, "42 22 14 10  5 12 21 19 32 23 38 36 70 60 55 46 45 52 59 66 69 95 79 75 87 82 92 98 96 " },
        { { 20 }, "42 22 14 10  5 12 20 19 21 32 23 38 36 70 60 55 46 45 52 59 66 69 95 79 75 87 82 92 98 96 " },
        { { 31 }, "42 22 14 10  5 12 20 19 21 32 23 31 38 36 70 60 55 46 45 52 59 66 69 95 79 75 87 82 92 98 96 " },
        { { 43 }, "42 22 14 10  5 12 20 19 21 32 23 31 38 36 70 60 46 45 43 55 52 59 66 69 95 79 75 87 82 92 98 96 " },
    };

    UTDataAVL tree;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_foreachPreorder(&tree, NULL) == CLIB_ERROR_INVALID_FUNCTION);
    CUT_CHECK(UTDataAVL_foreachPreorder(NULL, DataToString) == CLIB_ERROR_NULL_POINTER);

    for (size_t i = 0; i < _countof(testCases); i++) {
        CUT_CHECK(UTDataAVL_insert(&tree, &testCases[i].data) == CLIB_ERROR_SUCCESS);

        g_avlTraversalsBuffer[0] = 0;
        CUT_CHECK(UTDataAVL_foreachPreorder(&tree, DataToString) == CLIB_ERROR_SUCCESS);

        CUT_CHECK_OPERATOR_STRING(g_avlTraversalsBuffer, ==, testCases[i].expectedString);

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
    }

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

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

    CUT_CALL_TEST(test_avl_insert);
    CUT_CALL_TEST(test_avl_insertNode);

    CUT_CALL_TEST(test_avl_inorder);
    CUT_CALL_TEST(test_avl_postorder);
    CUT_CALL_TEST(test_avl_preorder);
CUT_END_MAIN
