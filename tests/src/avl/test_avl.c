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

    CUT_CHECK(tree.root == NULL);

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
    CUT_CHECK_OPERATOR_ADDRESS(tree.root, ==, NULL);

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

    AVLCopyDataSetError();
    CUT_CHECK(UTDataAVLNode_init(&node, &data) == AVL_ERROR_INTERNAL_ERROR);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

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

    AVLAllocNodeSetError();
    CUT_CHECK(UTDataAVLNode_create(&node, &data) == AVL_ERROR_NO_FREE_MEMORY);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    AVLCopyDataSetError();
    CUT_CHECK(UTDataAVLNode_create(&node, &data) == AVL_ERROR_INTERNAL_ERROR);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 1);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    CUT_CHECK(UTDataAVLNode_create(&node, &data) == AVL_ERROR_SUCCESS);

    CUT_CHECK(node != NULL);
    CUT_CHECK(node->left == NULL);
    CUT_CHECK(node->right == NULL);
    CUT_CHECK(node->height == 1);
    CUT_CHECK(node->data.data == 15);

    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 2);
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

static char avlInorderBuffer[2048];

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
		snprintf(numBuf, 64, "%u ", vec[i]);
		strcat(buffer, numBuf);
	}

	free(vec);
}

void avlInorderToString(const Data *data)
{
    char buf[64];
    snprintf(buf, 64, "%u ", data->data);
    strcat(avlInorderBuffer, buf);
}

CUT_DEFINE_TEST(test_avl_insert)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    unsigned int height;
    const Data *data = NULL;
    char buffer[2048];

    CUT_ASSERT(UTDataAVL_init(&tree) == AVL_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_insert(&tree, NULL) == AVL_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_insert(NULL, &g_insertTestCases[0].data) == AVL_ERROR_NULL_POINTER);

    AVLAllocNodeSetError();
    CUT_CHECK(UTDataAVL_insert(&tree, &g_insertTestCases[0].data) == AVL_ERROR_NO_FREE_MEMORY);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, 0);

    for (size_t i = 0; i < _countof(g_insertTestCases); i++) {
        CUT_CHECK(UTDataAVL_insert(&tree, &g_insertTestCases[i].data) == AVL_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, (i + 1) * sizeof(UTDataAVLNode));

        CUT_CHECK(UTDataAVL_getHeight(&tree, &height) == AVL_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(height, ==, g_insertTestCases[i].expectedHeight);

        CUT_CHECK(UTDataAVL_findType(&tree, &g_insertTestCases[i].data, &data) == AVL_ERROR_SUCCESS);

        getSubarraySortedAsString(buffer, i);
        avlInorderBuffer[0] = 0;

        CUT_CHECK(UTDataAVL_foreachInorder(&tree, avlInorderToString) == AVL_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_STRING(avlInorderBuffer, ==, buffer);
    }

    CUT_CHECK(UTDataAVL_free(&tree) == AVL_ERROR_SUCCESS);

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

    CUT_ASSERT(UTDataAVL_init(&tree) == AVL_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_insertNode(&tree, NULL) == AVL_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_insertNode(NULL, &unusedNode) == AVL_ERROR_NULL_POINTER);

    for (size_t i = 0; i < _countof(g_insertTestCases); i++) {
        CUT_CHECK(UTDataAVLNode_create(&node, &g_insertTestCases[i].data) == AVL_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, i + 1);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, (i + 1) * sizeof(UTDataAVLNode));

        CUT_CHECK(UTDataAVL_insertNode(&tree, node) == AVL_ERROR_SUCCESS);

        CUT_CHECK(UTDataAVL_getHeight(&tree, &height) == AVL_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_UINT32(height, ==, g_insertTestCases[i].expectedHeight);

        CUT_CHECK(UTDataAVL_findType(&tree, &g_insertTestCases[i].data, &data) == AVL_ERROR_SUCCESS);

        getSubarraySortedAsString(buffer, i);
        avlInorderBuffer[0] = 0;

        CUT_CHECK(UTDataAVL_foreachInorder(&tree, avlInorderToString) == AVL_ERROR_SUCCESS);
        CUT_CHECK_OPERATOR_STRING(avlInorderBuffer, ==, buffer);
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

    CUT_CALL_TEST(test_avl_insert);
    CUT_CALL_TEST(test_avl_insertNode);

CUT_END_MAIN
