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
        data.start = (unsigned int) i;

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

    data.start = 5;
    data.end   = 10;

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
    CUT_CHECK(node.data.start == 5);
    CUT_CHECK(node.data.end == 10);

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

    data.start = 20;
    data.end = 30;
    node.left = &left;
    node.right = &right;

    CUT_CHECK(UTDataAVLNode_uninit(NULL) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVLNode_uninit(&node) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(node.left == NULL);
    CUT_CHECK(node.right == NULL);
    CUT_CHECK(node.height == 0);
    CUT_CHECK(node.data.start == 0);
    CUT_CHECK(node.data.end == 0);

    CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, 0);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_node_create)
{
    RESET_STATISTICS;

    Data data;
    UTDataAVLNode *node = NULL;

    data.start = 100;
    data.end   = 200;

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
    CUT_CHECK(node->data.start == 100);
    CUT_CHECK(node->data.end == 200);

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

    data1.start = 500;
    data1.end   = 550;

    data2.start = 700;
    data2.end   = 800;

    data3.start = 2000;
    data3.end   = 3000;

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
        { { 61, 0 }, 1 },
        { { 58, 0 }, 2 },
        { { 41, 0 }, 2 },
        { {  5, 0 }, 3 },
        { { 29, 0 }, 3 },
        { { 17, 0 }, 3 },
        { { 25, 0 }, 3 },
        { { 97, 0 }, 4 },
        { { 87, 0 }, 4 },
        { { 57, 0 }, 4 },
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
    { {  125, 0 }, 1 }, { {  473, 0 }, 2 }, { {  213, 0 }, 2 }, { {  563, 0 }, 3 }, { {  477, 0 }, 3 },
    { {  910, 0 }, 3 }, { {   22, 0 }, 4 }, { {  159, 0 }, 4 }, { {  259, 0 }, 4 }, { {  371, 0 }, 4 },
    { {  274, 0 }, 4 }, { {  250, 0 }, 4 }, { {  281, 0 }, 5 }, { {   19, 0 }, 5 }, { {  841, 0 }, 5 },
    { {  153, 0 }, 5 }, { {  699, 0 }, 5 }, { {  939, 0 }, 5 }, { {   72, 0 }, 5 }, { {  920, 0 }, 5 },
    { {  744, 0 }, 5 }, { {  654, 0 }, 5 }, { {  346, 0 }, 5 }, { {  705, 0 }, 5 }, { {   94, 0 }, 6 },
    { {  573, 0 }, 6 }, { {   33, 0 }, 6 }, { {  192, 0 }, 6 }, { {  886, 0 }, 6 }, { {  472, 0 }, 6 },
    { {  494, 0 }, 6 }, { {  410, 0 }, 6 }, { {  717, 0 }, 6 }, { {  532, 0 }, 6 }, { {  954, 0 }, 6 },
    { {  883, 0 }, 6 }, { {  838, 0 }, 6 }, { {  847, 0 }, 7 }, { {  553, 0 }, 7 }, { {   30, 0 }, 7 },
    { {  190, 0 }, 7 }, { {  248, 0 }, 7 }, { {  426, 0 }, 7 }, { {  216, 0 }, 7 }, { {  721, 0 }, 7 },
    { {  196, 0 }, 7 }, { {  365, 0 }, 7 }, { {  548, 0 }, 7 }, { {   85, 0 }, 7 }, { {  748, 0 }, 7 },
    { {  737, 0 }, 7 }, { {  864, 0 }, 7 }, { {  987, 0 }, 7 }, { {  889, 0 }, 7 }, { {  948, 0 }, 7 },
    { {   70, 0 }, 7 }, { {  433, 0 }, 7 }, { {  975, 0 }, 7 }, { {   76, 0 }, 7 }, { {  759, 0 }, 7 },
    { {  320, 0 }, 7 }, { {  892, 0 }, 7 }, { {  219, 0 }, 7 }, { {   79, 0 }, 7 }, { {  819, 0 }, 7 },
    { {  829, 0 }, 7 }, { {  899, 0 }, 7 }, { {  999, 0 }, 7 }, { {  992, 0 }, 7 }, { {  405, 0 }, 8 },
    { {  683, 0 }, 8 }, { {  594, 0 }, 8 }, { {  148, 0 }, 8 }, { {  122, 0 }, 8 }, { {   61, 0 }, 8 },
    { {  695, 0 }, 8 }, { {  499, 0 }, 8 }, { {   42, 0 }, 8 }, { {  231, 0 }, 8 }, { {  215, 0 }, 8 },
    { {  475, 0 }, 8 }, { {  539, 0 }, 8 }, { {  100, 0 }, 8 }, { {  291, 0 }, 8 }, { {  158, 0 }, 8 },
    { {  128, 0 }, 8 }, { {  572, 0 }, 8 }, { {  497, 0 }, 8 }, { {  623, 0 }, 8 }, { {  449, 0 }, 8 },
    { {  550, 0 }, 8 }, { {  944, 0 }, 8 }, { {  456, 0 }, 8 }, { {  559, 0 }, 8 }, { {  935, 0 }, 8 },
    { {  199, 0 }, 8 }, { {  756, 0 }, 8 }, { {    7, 0 }, 8 }, { {  184, 0 }, 8 }, { {  617, 0 }, 8 },
    { {  380, 0 }, 8 }, { {  799, 0 }, 8 }, { {  490, 0 }, 8 }, { {  962, 0 }, 8 }, { {  120, 0 }, 8 },
    { {  746, 0 }, 8 }, { {  121, 0 }, 8 }, { {  798, 0 }, 8 }, { {  395, 0 }, 8 }, { {  154, 0 }, 8 },
    { {  201, 0 }, 8 }, { {  171, 0 }, 8 }, { {  601, 0 }, 8 }, { {  188, 0 }, 8 }, { {  926, 0 }, 8 },
    { {  609, 0 }, 8 }, { {  877, 0 }, 8 }, { {  716, 0 }, 8 }, { {  902, 0 }, 8 }, { {  901, 0 }, 8 },
    { {  809, 0 }, 8 }, { {  483, 0 }, 8 }, { {  801, 0 }, 8 }, { {  969, 0 }, 8 }, { {  495, 0 }, 8 },
    { {    9, 0 }, 8 }, { {   13, 0 }, 8 }, { {  704, 0 }, 8 }, { {  476, 0 }, 8 }, { {  189, 0 }, 8 },
    { {  599, 0 }, 8 }, { {  162, 0 }, 8 }, { {  951, 0 }, 8 }, { {   36, 0 }, 8 }, { {  738, 0 }, 8 },
    { {  747, 0 }, 8 }, { {   15, 0 }, 8 }, { {  710, 0 }, 8 }, { {  659, 0 }, 8 }, { {  648, 0 }, 8 },
    { {  341, 0 }, 8 }, { {  981, 0 }, 9 }, { {  399, 0 }, 9 }, { {  150, 0 }, 9 }, { {  740, 0 }, 9 },
    { {  261, 0 }, 9 }, { {  650, 0 }, 9 }, { {  963, 0 }, 9 }, { {  355, 0 }, 9 }, { {   91, 0 }, 9 },
    { {  933, 0 }, 9 }, { {  316, 0 }, 9 }, { {  308, 0 }, 9 }, { {  461, 0 }, 9 }, { {  228, 0 }, 9 },
    { {  354, 0 }, 9 }, { {  941, 0 }, 9 }, { {  618, 0 }, 9 }, { {  554, 0 }, 9 }, { {  796, 0 }, 9 },
    { {  638, 0 }, 9 }, { {  502, 0 }, 9 }, { {  868, 0 }, 9 }, { {  168, 0 }, 9 }, { {  692, 0 }, 9 },
    { {  420, 0 }, 9 }, { {  870, 0 }, 9 }, { {  783, 0 }, 9 }, { {  454, 0 }, 9 }, { {  790, 0 }, 9 },
    { {  455, 0 }, 9 }, { {  909, 0 }, 9 }, { {  272, 0 }, 9 }, { {  575, 0 }, 9 }, { {   46, 0 }, 9 },
    { {  544, 0 }, 9 }, { {  998, 0 }, 9 }, { {  412, 0 }, 9 }, { {   57, 0 }, 9 }, { {  388, 0 }, 9 },
    { {  132, 0 }, 9 }, { {  249, 0 }, 9 }, { {  549, 0 }, 9 }, { {   84, 0 }, 9 }, { {  394, 0 }, 9 },
    { {  459, 0 }, 9 }, { {  540, 0 }, 9 }, { {  156, 0 }, 9 }, { {  873, 0 }, 9 }, { {  774, 0 }, 9 },
    { {  940, 0 }, 9 }, { {  108, 0 }, 9 }, { {  663, 0 }, 9 }, { {  832, 0 }, 9 }, { {  891, 0 }, 9 },
    { {  210, 0 }, 9 }, { {  227, 0 }, 9 }, { {  616, 0 }, 9 }, { {    1, 0 }, 9 }, { {  260, 0 }, 9 },
};

static char g_avlTraversalsBuffer[2048];

int DataSortCmp(const void *a, const void *b)
{
	return ( (*((unsigned int*)a))  - (*((unsigned int*)b)) );
}

void getSubarraySortedAsString(char *buffer, size_t pos)
{
	buffer[0] = 0;
    
	unsigned int *vec = (unsigned int*)malloc(sizeof(unsigned int) * (pos + 1));
	if (vec == NULL) {
		return;
	}

	for (size_t i = 0; i <= pos; i++) {
		vec[i] = g_insertTestCases[i].data.start;
	}

	qsort(vec, pos + 1, sizeof(unsigned int), DataSortCmp);

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
    snprintf(buf, 64, "%2u ", data->start);
    strcat(g_avlTraversalsBuffer, buf);
}

CUT_DEFINE_TEST(test_avl_insert)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    unsigned int height;
    const Data *data = NULL;
    char buffer[2048];
    clib_bool_t isBalanced;

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

        CUT_CHECK(UTDataAVL_isBalanced(&tree, &isBalanced) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(isBalanced == CLIB_TRUE);
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
    clib_bool_t isBalanced;

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

        CUT_CHECK(UTDataAVL_isBalanced(&tree, &isBalanced) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(isBalanced == CLIB_TRUE);
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
        { { 70, 0 }, "70 " },
        { { 75, 0 }, "70 75 " },
        { { 32, 0 }, "32 70 75 " },
        { { 12, 0 }, "12 32 70 75 " },
        { { 79, 0 }, "12 32 70 75 79 " },
        { { 55, 0 }, "12 32 55 70 75 79 " },
        { { 98, 0 }, "12 32 55 70 75 79 98 " },
        { { 42, 0 }, "12 32 42 55 70 75 79 98 " },
        { {  5, 0 }, " 5 12 32 42 55 70 75 79 98 " },
        { { 38, 0 }, " 5 12 32 38 42 55 70 75 79 98 " },
        { { 60, 0 }, " 5 12 32 38 42 55 60 70 75 79 98 " },
        { { 66, 0 }, " 5 12 32 38 42 55 60 66 70 75 79 98 " },
        { { 69, 0 }, " 5 12 32 38 42 55 60 66 69 70 75 79 98 " },
        { { 36, 0 }, " 5 12 32 36 38 42 55 60 66 69 70 75 79 98 " },
        { { 87, 0 }, " 5 12 32 36 38 42 55 60 66 69 70 75 79 87 98 " },
        { { 14, 0 }, " 5 12 14 32 36 38 42 55 60 66 69 70 75 79 87 98 " },
        { { 23, 0 }, " 5 12 14 23 32 36 38 42 55 60 66 69 70 75 79 87 98 " },
        { { 95, 0 }, " 5 12 14 23 32 36 38 42 55 60 66 69 70 75 79 87 95 98 " },
        { { 96, 0 }, " 5 12 14 23 32 36 38 42 55 60 66 69 70 75 79 87 95 96 98 " },
        { { 52, 0 }, " 5 12 14 23 32 36 38 42 52 55 60 66 69 70 75 79 87 95 96 98 " },
        { { 22, 0 }, " 5 12 14 22 23 32 36 38 42 52 55 60 66 69 70 75 79 87 95 96 98 " },
        { { 21, 0 }, " 5 12 14 21 22 23 32 36 38 42 52 55 60 66 69 70 75 79 87 95 96 98 " },
        { { 59, 0 }, " 5 12 14 21 22 23 32 36 38 42 52 55 59 60 66 69 70 75 79 87 95 96 98 " },
        { { 82, 0 }, " 5 12 14 21 22 23 32 36 38 42 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 19, 0 }, " 5 12 14 19 21 22 23 32 36 38 42 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 10, 0 }, " 5 10 12 14 19 21 22 23 32 36 38 42 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 45, 0 }, " 5 10 12 14 19 21 22 23 32 36 38 42 45 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 46, 0 }, " 5 10 12 14 19 21 22 23 32 36 38 42 45 46 52 55 59 60 66 69 70 75 79 82 87 95 96 98 " },
        { { 92, 0 }, " 5 10 12 14 19 21 22 23 32 36 38 42 45 46 52 55 59 60 66 69 70 75 79 82 87 92 95 96 98 " },
        { { 20, 0 }, " 5 10 12 14 19 20 21 22 23 32 36 38 42 45 46 52 55 59 60 66 69 70 75 79 82 87 92 95 96 98 " },
        { { 31, 0 }, " 5 10 12 14 19 20 21 22 23 31 32 36 38 42 45 46 52 55 59 60 66 69 70 75 79 82 87 92 95 96 98 " },
        { { 43, 0 }, " 5 10 12 14 19 20 21 22 23 31 32 36 38 42 43 45 46 52 55 59 60 66 69 70 75 79 82 87 92 95 96 98 " },
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
        { { 70, 0 }, "70 " },
        { { 75, 0 }, "75 70 " },
        { { 32, 0 }, "32 75 70 " },
        { { 12, 0 }, "12 32 75 70 " },
        { { 79, 0 }, "12 32 79 75 70 " },
        { { 55, 0 }, "12 55 32 79 75 70 " },
        { { 98, 0 }, "12 55 32 75 98 79 70 " },
        { { 42, 0 }, "12 42 55 32 75 98 79 70 " },
        { {  5, 0 }, " 5 12 42 55 32 75 98 79 70 " },
        { { 38, 0 }, " 5 12 38 55 42 32 75 98 79 70 " },
        { { 60, 0 }, " 5 12 38 32 60 55 75 98 79 70 42 " },
        { { 66, 0 }, " 5 12 38 32 55 66 60 75 98 79 70 42 " },
        { { 69, 0 }, " 5 12 38 32 55 69 66 60 75 98 79 70 42 " },
        { { 36, 0 }, " 5 12 36 38 32 55 69 66 60 75 98 79 70 42 " },
        { { 87, 0 }, " 5 12 36 38 32 55 69 66 60 75 87 98 79 70 42 " },
        { { 14, 0 }, " 5 14 12 36 38 32 55 69 66 60 75 87 98 79 70 42 " },
        { { 23, 0 }, " 5 23 14 12 36 38 32 55 69 66 60 75 87 98 79 70 42 " },
        { { 95, 0 }, " 5 23 14 12 36 38 32 55 69 66 60 75 87 98 95 79 70 42 " },
        { { 96, 0 }, " 5 23 14 12 36 38 32 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 52, 0 }, " 5 23 14 12 36 38 32 52 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 22, 0 }, " 5 14 23 22 12 36 38 32 52 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 21, 0 }, " 5 12 21 23 22 14 36 38 32 52 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 59, 0 }, " 5 12 21 23 22 14 36 38 32 52 59 55 69 66 60 75 87 79 96 98 95 70 42 " },
        { { 82, 0 }, " 5 12 21 23 22 14 36 38 32 52 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 19, 0 }, " 5 12 19 21 14 23 36 38 32 22 52 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 10, 0 }, " 5 12 10 19 21 14 23 36 38 32 22 52 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 45, 0 }, " 5 12 10 19 21 14 23 36 38 32 22 45 52 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 46, 0 }, " 5 12 10 19 21 14 23 36 38 32 22 45 52 46 59 55 69 66 60 75 82 87 79 96 98 95 70 42 " },
        { { 92, 0 }, " 5 12 10 19 21 14 23 36 38 32 22 45 52 46 59 55 69 66 60 75 82 92 87 79 96 98 95 70 42 " },
        { { 20, 0 }, " 5 12 10 19 21 20 14 23 36 38 32 22 45 52 46 59 55 69 66 60 75 82 92 87 79 96 98 95 70 42 " },
        { { 31, 0 }, " 5 12 10 19 21 20 14 31 23 36 38 32 22 45 52 46 59 55 69 66 60 75 82 92 87 79 96 98 95 70 42 " },
        { { 43, 0 }, " 5 12 10 19 21 20 14 31 23 36 38 32 22 43 45 52 59 55 46 69 66 60 75 82 92 87 79 96 98 95 70 42 " },
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
        { { 70, 0 }, "70 " },
        { { 75, 0 }, "70 75 " },
        { { 32, 0 }, "70 32 75 " },
        { { 12, 0 }, "70 32 12 75 " },
        { { 79, 0 }, "70 32 12 75 79 " },
        { { 55, 0 }, "70 32 12 55 75 79 " },
        { { 98, 0 }, "70 32 12 55 79 75 98 " },
        { { 42, 0 }, "70 32 12 55 42 79 75 98 " },
        { {  5, 0 }, "70 32 12  5 55 42 79 75 98 " },
        { { 38, 0 }, "70 32 12  5 42 38 55 79 75 98 " },
        { { 60, 0 }, "42 32 12  5 38 70 55 60 79 75 98 " },
        { { 66, 0 }, "42 32 12  5 38 70 60 55 66 79 75 98 " },
        { { 69, 0 }, "42 32 12  5 38 70 60 55 66 69 79 75 98 " },
        { { 36, 0 }, "42 32 12  5 38 36 70 60 55 66 69 79 75 98 " },
        { { 87, 0 }, "42 32 12  5 38 36 70 60 55 66 69 79 75 98 87 " },
        { { 14, 0 }, "42 32 12  5 14 38 36 70 60 55 66 69 79 75 98 87 " },
        { { 23, 0 }, "42 32 12  5 14 23 38 36 70 60 55 66 69 79 75 98 87 " },
        { { 95, 0 }, "42 32 12  5 14 23 38 36 70 60 55 66 69 79 75 95 87 98 " },
        { { 96, 0 }, "42 32 12  5 14 23 38 36 70 60 55 66 69 95 79 75 87 98 96 " },
        { { 52, 0 }, "42 32 12  5 14 23 38 36 70 60 55 52 66 69 95 79 75 87 98 96 " },
        { { 22, 0 }, "42 32 12  5 22 14 23 38 36 70 60 55 52 66 69 95 79 75 87 98 96 " },
        { { 21, 0 }, "42 32 14 12  5 22 21 23 38 36 70 60 55 52 66 69 95 79 75 87 98 96 " },
        { { 59, 0 }, "42 32 14 12  5 22 21 23 38 36 70 60 55 52 59 66 69 95 79 75 87 98 96 " },
        { { 82, 0 }, "42 32 14 12  5 22 21 23 38 36 70 60 55 52 59 66 69 95 79 75 87 82 98 96 " },
        { { 19, 0 }, "42 22 14 12  5 21 19 32 23 38 36 70 60 55 52 59 66 69 95 79 75 87 82 98 96 " },
        { { 10, 0 }, "42 22 14 10  5 12 21 19 32 23 38 36 70 60 55 52 59 66 69 95 79 75 87 82 98 96 " },
        { { 45, 0 }, "42 22 14 10  5 12 21 19 32 23 38 36 70 60 55 52 45 59 66 69 95 79 75 87 82 98 96 " },
        { { 46, 0 }, "42 22 14 10  5 12 21 19 32 23 38 36 70 60 55 46 45 52 59 66 69 95 79 75 87 82 98 96 " },
        { { 92, 0 }, "42 22 14 10  5 12 21 19 32 23 38 36 70 60 55 46 45 52 59 66 69 95 79 75 87 82 92 98 96 " },
        { { 20, 0 }, "42 22 14 10  5 12 20 19 21 32 23 38 36 70 60 55 46 45 52 59 66 69 95 79 75 87 82 92 98 96 " },
        { { 31, 0 }, "42 22 14 10  5 12 20 19 21 32 23 31 38 36 70 60 55 46 45 52 59 66 69 95 79 75 87 82 92 98 96 " },
        { { 43, 0 }, "42 22 14 10  5 12 20 19 21 32 23 31 38 36 70 60 46 45 43 55 52 59 66 69 95 79 75 87 82 92 98 96 " },
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

CUT_DEFINE_TEST(test_avl_find)
{
    RESET_STATISTICS;

    static unsigned int elements[] = { 32, 89, 26, 31, 33, 42, 34, 94, 84, 43, 86, 13, 20, 71, 4, 96, 48, 64, 77, 62 };

    UTDataAVL tree;
    const UTDataAVLNode *result = NULL;
    Data value;

    value.start = 1;
    value.end = 0;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_find(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
    CUT_CHECK(result == NULL);

    for (size_t i = 0; i < _countof(elements); i++) {
        Data tmp;
        tmp.start = elements[i];
        tmp.end = 0;

        CUT_CHECK(UTDataAVL_insert(&tree, &tmp) == CLIB_ERROR_SUCCESS);
    }

    CUT_CHECK(UTDataAVL_find(NULL, &value, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_find(&tree, NULL, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_find(&tree, &value, NULL) == CLIB_ERROR_NULL_POINTER);

    for (size_t i = 0; i < (size_t) 100; i++) {
        value.start = i;
        value.end = 0;

        int exists = 0;
        for (size_t j = 0; j < _countof(elements); j++) {
            if (i == elements[j]) {
                exists = 1;
                break;
            }
        }

        if (exists == 1) {
            CUT_CHECK(UTDataAVL_find(&tree, &value, &result) == CLIB_ERROR_SUCCESS);
            CUT_ASSERT(result != NULL);
            CUT_CHECK(result->data.start == i);
            CUT_CHECK(result->data.end == 0);
        } else {
            CUT_CHECK(UTDataAVL_find(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(result == NULL);
        }
    }

    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, _countof(elements));
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, _countof(elements));
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, _countof(elements) * sizeof(UTDataAVLNode));

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_findType)
{
    RESET_STATISTICS;

    static unsigned int elements[] = { 30, 24, 18, 6, 49, 51, 52, 97, 79, 39, 41, 73, 67, 12, 87, 10, 72, 46, 91, 57 };

    UTDataAVL tree;
    const Data *result = NULL;
    Data value;

    value.start = 1;
    value.end = 0;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_findType(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
    CUT_CHECK(result == NULL);

    for (size_t i = 0; i < _countof(elements); i++) {
        Data tmp;
        tmp.start = elements[i];
        tmp.end = 0;

        CUT_CHECK(UTDataAVL_insert(&tree, &tmp) == CLIB_ERROR_SUCCESS);
    }

    CUT_CHECK(UTDataAVL_findType(NULL, &value, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_findType(&tree, NULL, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_findType(&tree, &value, NULL) == CLIB_ERROR_NULL_POINTER);

    for (size_t i = 0; i < (size_t) 100; i++) {
        value.start = i;
        value.end = 0;

        int exists = 0;
        for (size_t j = 0; j < _countof(elements); j++) {
            if (i == elements[j]) {
                exists = 1;
                break;
            }
        }

        if (exists == 1) {
            CUT_CHECK(UTDataAVL_findType(&tree, &value, &result) == CLIB_ERROR_SUCCESS);
            CUT_ASSERT(result != NULL);
            CUT_CHECK(result->start == i);
            CUT_CHECK(result->end == 0);
        } else {
            CUT_CHECK(UTDataAVL_findType(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(result == NULL);
        }
    }

    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, _countof(elements));
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, _countof(elements));
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, _countof(elements) * sizeof(UTDataAVLNode));

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_findGreaterOrEqual)
{
    RESET_STATISTICS;

    static unsigned int elements[] = { 70, 76, 83, 69, 60, 14, 92, 22, 61, 38, 53, 21, 63, 74, 3, 2, 78, 35, 58, 68 };

    UTDataAVL tree;
    const UTDataAVLNode *result = NULL;
    Data value;

    value.start = 1;
    value.end = 0;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_findGreaterOrEqual(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
    CUT_CHECK(result == NULL);

    for (size_t i = 0; i < _countof(elements); i++) {
        Data tmp;
        tmp.start = elements[i];
        tmp.end = 0;

        CUT_CHECK(UTDataAVL_insert(&tree, &tmp) == CLIB_ERROR_SUCCESS);
    }

    CUT_CHECK(UTDataAVL_findGreaterOrEqual(NULL, &value, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_findGreaterOrEqual(&tree, NULL, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(UTDataAVL_findGreaterOrEqual(&tree, &value, NULL) == CLIB_ERROR_NULL_POINTER);

    qsort(elements, _countof(elements), sizeof(unsigned int), DataSortCmp);

    for (size_t i = 0; i < (size_t) 100; i++) {
        value.start = i;
        value.end = 0;

        int exists = 0;
        size_t j;
        for (j = 0; j < _countof(elements); j++) {
            if (elements[j] >= i) {
                exists = 1;
                break;
            }
        }

        if (exists == 1) {
            CUT_CHECK(UTDataAVL_findGreaterOrEqual(&tree, &value, &result) == CLIB_ERROR_SUCCESS);
            CUT_ASSERT(result != NULL);
            CUT_CHECK(result->data.start == elements[j]);
            CUT_CHECK(result->data.end == 0);
        } else {
            CUT_CHECK(UTDataAVL_findGreaterOrEqual(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(result == NULL);
        }
    }

    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, _countof(elements));
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, _countof(elements));
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, _countof(elements) * sizeof(UTDataAVLNode));

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

static unsigned int g_testRemove_lastValue;
static int g_testRemove_sorted;

void TestRmoveCheckValue(const Data *data)
{
    if (data == NULL) {
        g_testRemove_sorted = 0;
        return;
    }

    if (data->start < g_testRemove_lastValue) {
        g_testRemove_sorted = 0;
        return;
    }
}

CUT_DEFINE_TEST(test_avl_remove)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    UTDataAVLNode *result = NULL;
    Data value;
    const Data *findResult = NULL;
    clib_bool_t isBalanced;
    size_t freeCalls = 0;
    size_t objectsInUsage;

    value.start = 1;
    value.end = 0;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_remove(&tree, &value, NULL) == CLIB_ERROR_NULL_POINTER);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(NULL, &value, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(&tree, NULL, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
    CUT_CHECK(result == NULL);

    // create tree
    for (size_t i = 0; i < _countof(g_insertTestCases); i++) {
        value.start = g_insertTestCases[i].data.start;
        CUT_CHECK(UTDataAVL_insert(&tree, &value) == CLIB_ERROR_SUCCESS);
    }
    objectsInUsage = _countof(g_insertTestCases);

    for (size_t i = 0; i < (size_t) 1000; i++) {
        int exists = 0;
        for (size_t j = 0; j < _countof(g_insertTestCases); j++) {
            if (i == g_insertTestCases[j].data.start) {
                exists = 1;
                break;
            }
        }

        value.start = i;

        if (exists == 1) {
            result = NULL;
            CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_SUCCESS);

            CUT_ASSERT(result != NULL);
            CUT_CHECK(result->data.start == i);

            findResult = (const Data*) 0x0000FFFF;
            CUT_CHECK(UTDataAVL_findType(&tree, &value, &findResult) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(findResult == NULL);

            isBalanced = CLIB_FALSE;
            CUT_CHECK(UTDataAVL_isBalanced(&tree, &isBalanced) == CLIB_ERROR_SUCCESS);
            CUT_CHECK(isBalanced == CLIB_TRUE);

            CUT_CHECK(UTDataAVLNode_free(result) == CLIB_ERROR_SUCCESS);

            freeCalls++;
            objectsInUsage--;
        } else {
            result = (UTDataAVLNode*) 0x0000FFFF;
            CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(result == NULL);

            isBalanced = CLIB_FALSE;
            CUT_CHECK(UTDataAVL_isBalanced(&tree, &isBalanced) == CLIB_ERROR_SUCCESS);
            CUT_CHECK(isBalanced == CLIB_TRUE);
        }

        g_testRemove_lastValue = 0;
        g_testRemove_sorted = 1;
        CUT_CHECK(UTDataAVL_foreachInorder(&tree, TestRmoveCheckValue) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(g_testRemove_sorted == 1);

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, _countof(g_insertTestCases));
        CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, freeCalls);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, objectsInUsage);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, objectsInUsage * sizeof(UTDataAVLNode));
    }

    CUT_CHECK(tree.root == NULL);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_remove_reverse)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    UTDataAVLNode *result = NULL;
    Data value;
    const Data *findResult = NULL;
    clib_bool_t isBalanced;
    size_t freeCalls = 0;
    size_t objectsInUsage;

    value.start = 1;
    value.end = 0;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_remove(&tree, &value, NULL) == CLIB_ERROR_NULL_POINTER);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(NULL, &value, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(&tree, NULL, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
    CUT_CHECK(result == NULL);

    // create tree
    for (size_t i = 0; i < _countof(g_insertTestCases); i++) {
        value.start = g_insertTestCases[i].data.start;
        CUT_CHECK(UTDataAVL_insert(&tree, &value) == CLIB_ERROR_SUCCESS);
    }
    objectsInUsage = _countof(g_insertTestCases);

    for (size_t i = 1000; i > 0; i--) {
        int exists = 0;
        for (size_t j = 0; j < _countof(g_insertTestCases); j++) {
            if (i - 1 == g_insertTestCases[j].data.start) {
                exists = 1;
                break;
            }
        }

        value.start = i - 1;

        if (exists == 1) {
            result = NULL;
            CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_SUCCESS);

            CUT_ASSERT(result != NULL);
            CUT_CHECK(result->data.start == i - 1);

            findResult = (const Data*) 0x0000FFFF;
            CUT_CHECK(UTDataAVL_findType(&tree, &value, &findResult) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(findResult == NULL);

            isBalanced = CLIB_FALSE;
            CUT_CHECK(UTDataAVL_isBalanced(&tree, &isBalanced) == CLIB_ERROR_SUCCESS);
            CUT_CHECK(isBalanced == CLIB_TRUE);

            CUT_CHECK(UTDataAVLNode_free(result) == CLIB_ERROR_SUCCESS);

            freeCalls++;
            objectsInUsage--;
        } else {
            result = (UTDataAVLNode*) 0x0000FFFF;
            CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(result == NULL);

            isBalanced = CLIB_FALSE;
            CUT_CHECK(UTDataAVL_isBalanced(&tree, &isBalanced) == CLIB_ERROR_SUCCESS);
            CUT_CHECK(isBalanced == CLIB_TRUE);
        }

        g_testRemove_lastValue = 0;
        g_testRemove_sorted = 1;
        CUT_CHECK(UTDataAVL_foreachInorder(&tree, TestRmoveCheckValue) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(g_testRemove_sorted == 1);

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, _countof(g_insertTestCases));
        CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, freeCalls);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, objectsInUsage);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, objectsInUsage * sizeof(UTDataAVLNode));
    }

    CUT_CHECK(tree.root == NULL);

    CHECK_STATISTICS;
}

void shuffle(unsigned int *a_array, size_t a_length)
{
    if (a_length < 2) {
        return;
    }

    for (size_t it = 0; it < a_length; it++) {
        size_t i = 0, j = 0;
        while (i == j) {
            i = ((size_t) rand()) % a_length;
            j = ((size_t) rand()) % a_length;
        }

        unsigned int tmp = a_array[j];
        a_array[j] = a_array[i];
        a_array[i] = tmp;
    }
}

CUT_DEFINE_TEST(test_avl_remove_random)
{
    RESET_STATISTICS;

    UTDataAVL tree;
    UTDataAVLNode *result = NULL;
    Data value;
    const Data *findResult = NULL;
    clib_bool_t isBalanced;
    size_t freeCalls = 0;
    size_t objectsInUsage;

    value.start = 1;
    value.end = 0;

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_remove(&tree, &value, NULL) == CLIB_ERROR_NULL_POINTER);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(NULL, &value, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(&tree, NULL, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
    CUT_CHECK(result == NULL);

    // create tree
    for (size_t i = 0; i < _countof(g_insertTestCases); i++) {
        value.start = g_insertTestCases[i].data.start;
        CUT_CHECK(UTDataAVL_insert(&tree, &value) == CLIB_ERROR_SUCCESS);
    }
    objectsInUsage = _countof(g_insertTestCases);

    unsigned int randomValues[1000];
    for (size_t i = 0; i < 1000; i++) {
        randomValues[i] = i;
    }

    shuffle(randomValues, 1000);

    for (size_t i = 0; i < (size_t) 1000; i++) {
        int exists = 0;
        for (size_t j = 0; j < _countof(g_insertTestCases); j++) {
            if (randomValues[i] == g_insertTestCases[j].data.start) {
                exists = 1;
                break;
            }
        }

        value.start = randomValues[i];

        if (exists == 1) {
            result = NULL;
            CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_SUCCESS);

            CUT_ASSERT(result != NULL);
            CUT_CHECK(result->data.start == randomValues[i]);

            findResult = (const Data*) 0x0000FFFF;
            CUT_CHECK(UTDataAVL_findType(&tree, &value, &findResult) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(findResult == NULL);

            isBalanced = CLIB_FALSE;
            CUT_CHECK(UTDataAVL_isBalanced(&tree, &isBalanced) == CLIB_ERROR_SUCCESS);
            CUT_CHECK(isBalanced == CLIB_TRUE);

            CUT_CHECK(UTDataAVLNode_free(result) == CLIB_ERROR_SUCCESS);

            freeCalls++;
            objectsInUsage--;
        } else {
            result = (UTDataAVLNode*) 0x0000FFFF;
            CUT_CHECK(UTDataAVL_remove(&tree, &value, &result) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(result == NULL);

            isBalanced = CLIB_FALSE;
            CUT_CHECK(UTDataAVL_isBalanced(&tree, &isBalanced) == CLIB_ERROR_SUCCESS);
            CUT_CHECK(isBalanced == CLIB_TRUE);
        }

        g_testRemove_lastValue = 0;
        g_testRemove_sorted = 1;
        CUT_CHECK(UTDataAVL_foreachInorder(&tree, TestRmoveCheckValue) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(g_testRemove_sorted == 1);

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, _countof(g_insertTestCases));
        CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, freeCalls);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, objectsInUsage);
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, objectsInUsage * sizeof(UTDataAVLNode));
    }

    CUT_CHECK(tree.root == NULL);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_isBalanced)
{
    RESET_STATISTICS;

    {
        UTDataAVL tree;
        clib_bool_t result = CLIB_FALSE;

        CUT_CHECK(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

        CUT_CHECK(UTDataAVL_isBalanced(&tree, NULL) == CLIB_ERROR_NULL_POINTER);
        CUT_CHECK(UTDataAVL_isBalanced(NULL, &result) == CLIB_ERROR_NULL_POINTER);

        CUT_CHECK(UTDataAVL_isBalanced(&tree, &result) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(result == CLIB_TRUE);
    }

    {
        UTDataAVL tree;
        clib_bool_t result = CLIB_TRUE;
        UTDataAVLNode parent, left, right;

        parent.height = 0;
        parent.data.start = 0;
        parent.data.end = 0;
        parent.left = &left;
        parent.right = &right;

        left.height = 5;
        left.data.start = 0;
        left.data.end = 0;
        left.left = CLIB_NULLPTR;
        left.right = CLIB_NULLPTR;

        right.height = 1;
        right.data.start = 0;
        right.data.end = 0;
        right.left = CLIB_NULLPTR;
        right.right = CLIB_NULLPTR;

        CUT_CHECK(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

        tree.root = &parent;

        CUT_CHECK(UTDataAVL_isBalanced(&tree, &result) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(result == CLIB_FALSE);
    }

    {
        UTDataAVL tree;
        clib_bool_t result = CLIB_TRUE;
        UTDataAVLNode parent, left, right;

        parent.height = 0;
        parent.data.start = 0;
        parent.data.end = 0;
        parent.left = &left;
        parent.right = &right;

        left.height = 50;
        left.data.start = 0;
        left.data.end = 0;
        left.left = CLIB_NULLPTR;
        left.right = CLIB_NULLPTR;

        right.height = 100;
        right.data.start = 0;
        right.data.end = 0;
        right.left = CLIB_NULLPTR;
        right.right = CLIB_NULLPTR;

        CUT_CHECK(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

        tree.root = &parent;

        CUT_CHECK(UTDataAVL_isBalanced(&tree, &result) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(result == CLIB_FALSE);
    }

    {
        UTDataAVL tree;
        clib_bool_t result = CLIB_TRUE;
        UTDataAVLNode parent, left, right, leftLeft, leftRight;

        parent.height = 2;
        parent.data.start = 0;
        parent.data.end = 0;
        parent.left = &left;
        parent.right = &right;

        left.height = 2;
        left.data.start = 0;
        left.data.end = 0;
        left.left = &leftLeft;
        left.right = &leftRight;

        right.height = 1;
        right.data.start = 0;
        right.data.end = 0;
        right.left = CLIB_NULLPTR;
        right.right = CLIB_NULLPTR;

        leftLeft.height = 5;
        leftLeft.data.start = 0;
        leftLeft.data.end = 0;
        leftLeft.left = CLIB_NULLPTR;
        leftLeft.right = CLIB_NULLPTR;

        leftRight.height = 7;
        leftRight.data.start = 0;
        leftRight.data.end = 0;
        leftRight.left = CLIB_NULLPTR;
        leftRight.right = CLIB_NULLPTR;

        CUT_CHECK(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

        tree.root = &parent;

        CUT_CHECK(UTDataAVL_isBalanced(&tree, &result) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(result == CLIB_FALSE);
    }

    {
        UTDataAVL tree;
        clib_bool_t result = CLIB_TRUE;
        UTDataAVLNode parent, left, right, rightLeft, rightRight;

        parent.height = 2;
        parent.data.start = 0;
        parent.data.end = 0;
        parent.left = &left;
        parent.right = &right;

        left.height = 1;
        left.data.start = 0;
        left.data.end = 0;
        left.left = CLIB_NULLPTR;
        left.right = CLIB_NULLPTR;

        right.height = 2;
        right.data.start = 0;
        right.data.end = 0;
        right.left = &rightLeft;
        right.right = &rightRight;

        rightLeft.height = 19;
        rightLeft.data.start = 0;
        rightLeft.data.end = 0;
        rightLeft.left = CLIB_NULLPTR;
        rightLeft.right = CLIB_NULLPTR;

        rightRight.height = 15;
        rightRight.data.start = 0;
        rightRight.data.end = 0;
        rightRight.left = CLIB_NULLPTR;
        rightRight.right = CLIB_NULLPTR;

        CUT_CHECK(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

        tree.root = &parent;

        CUT_CHECK(UTDataAVL_isBalanced(&tree, &result) == CLIB_ERROR_SUCCESS);
        CUT_CHECK(result == CLIB_FALSE);
    }

    CHECK_STATISTICS;
}

void GenerateRandomArray(unsigned int *a_array, size_t a_length, unsigned int a_start, unsigned int a_end)
{
    unsigned int diff = a_end - a_start;
    int state;

    for (size_t i = 0; i < a_length; i++) {
        state = 1;
        do {
            unsigned int num = (unsigned int) rand();
            num %= diff;
            num += a_start;

            for (size_t j = 0; j < i; j++) {
                if (a_array[j] == num) {
                    state++;
                    break;
                }
            }

            if (state == 2) {
                state = 1;
            } else {
                state = 0;
                a_array[i] = num;
            }

        } while (state);
    }
}

clib_bool_t TestFindIfCbk(const Data *a_value, void *a_context)
{
    Data *expectedData = (Data*) a_context;
    if (a_value->start == expectedData->start) {
        return CLIB_TRUE;
    }

    return CLIB_FALSE;
}

CUT_DEFINE_TEST(test_avl_findIf)
{
    RESET_STATISTICS;

    unsigned int values[200] = { 0 };
    UTDataAVL tree;
    Data data;
    const UTDataAVLNode *result = NULL;

    data.start = 0;
    data.end = 0;

    GenerateRandomArray(values, _countof(values), 0, 1000);

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_findIf(&tree, TestFindIfCbk, &data, NULL) == CLIB_ERROR_NULL_POINTER);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_findIf(NULL, TestFindIfCbk, &data, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_findIf(&tree, NULL, &data, &result) == CLIB_ERROR_INVALID_FUNCTION);
    CUT_CHECK(result == NULL);

    result = (UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_findIf(&tree, TestFindIfCbk, &data, &result) == CLIB_ERROR_NOT_FOUND);
    CUT_CHECK(result == NULL);

    for (size_t i = 0; i < _countof(values); i++) {
        data.start = values[i];
        CUT_CHECK(UTDataAVL_insert(&tree, &data) == CLIB_ERROR_SUCCESS);
    }

    for (size_t i = 0; i < (size_t) 1000; i++) {
        data.start = (unsigned int) i;

        int exists = 0;
        for (size_t j = 0; j < _countof(values); j++) {
            if (data.start == values[j]) {
                exists = 1;
                break;
            }
        }

        if (exists == 1) {
            result = NULL;
            CUT_CHECK(UTDataAVL_findIf(&tree, TestFindIfCbk, &data, &result) == CLIB_ERROR_SUCCESS);
            CUT_ASSERT(result != NULL);

            CUT_CHECK(result->data.start == data.start);
            CUT_CHECK(result->data.end == data.end);
        } else {
            result = (const UTDataAVLNode*) 0x0000FFFF;
            CUT_CHECK(UTDataAVL_findIf(&tree, TestFindIfCbk, &data, &result) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(result == NULL);
        }

        CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, _countof(values));
        CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, 0);
        CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, _countof(values));
        CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, _countof(values) * sizeof(UTDataAVLNode));
    }

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_TEST(test_avl_findLessOrEqual)
{
    RESET_STATISTICS;

    unsigned int values[200] = { 0 };
    UTDataAVL tree;
    Data data;
    const UTDataAVLNode *result = NULL;

    data.start = 0;
    data.end = 0;

    GenerateRandomArray(values, _countof(values), 50, 950);

    CUT_ASSERT(UTDataAVL_init(&tree) == CLIB_ERROR_SUCCESS);

    CUT_CHECK(UTDataAVL_findLessOrEqual(&tree, &data, NULL) == CLIB_ERROR_NULL_POINTER);

    result = (const UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_findLessOrEqual(NULL, &data, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (const UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_findLessOrEqual(&tree, NULL, &result) == CLIB_ERROR_NULL_POINTER);
    CUT_CHECK(result == NULL);

    result = (const UTDataAVLNode*) 0x0000FFFF;
    CUT_CHECK(UTDataAVL_findLessOrEqual(&tree, &data, &result) == CLIB_ERROR_NOT_FOUND);
    CUT_CHECK(result == NULL);

    for (size_t i = 0; i < _countof(values); i++) {
        data.start = values[i];
        CUT_CHECK(UTDataAVL_insert(&tree, &data) == CLIB_ERROR_SUCCESS);
    }

    qsort(values, _countof(values), sizeof(unsigned int), DataSortCmp);

    for (unsigned int i = 0; i < 1000; i++) {
        data.start = i;
        data.end = 0;

        int exists = 0;
        size_t j;
        for (j = _countof(values); j > 0; j--) {
            if (i >= values[j - 1]) {
                exists = 1;
                break;
            }
        }

        if (exists == 1) {
            result = NULL;
            CUT_CHECK(UTDataAVL_findLessOrEqual(&tree, &data, &result) == CLIB_ERROR_SUCCESS);
            CUT_ASSERT(result != NULL);
            CUT_CHECK(result->data.start == values[j - 1]);
            CUT_CHECK(result->data.end == 0);
        } else {
            result = (const UTDataAVLNode*) 0x0000FFFF;
            CUT_CHECK(UTDataAVL_findLessOrEqual(&tree, &data, &result) == CLIB_ERROR_NOT_FOUND);
            CUT_CHECK(result == NULL);
        }
    }

    CUT_CHECK_OPERATOR_SIZE_T(GetFreeCalls(), ==, 0);
    CUT_CHECK_OPERATOR_SIZE_T(GetAllocCalls(), ==, _countof(values));
    CUT_CHECK_OPERATOR_SIZE_T(GetObjectsInUsage(), ==, _countof(values));
    CUT_CHECK_OPERATOR_SIZE_T(GetMemoryInUsage(), ==, _countof(values) * sizeof(UTDataAVLNode));

    CUT_CHECK(UTDataAVL_free(&tree) == CLIB_ERROR_SUCCESS);

    CHECK_STATISTICS;
}

CUT_DEFINE_MAIN

    // initializes with a constant value to always generate the same values.
    srand(100);

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

    CUT_CALL_TEST(test_avl_find);
    CUT_CALL_TEST(test_avl_findType);
    CUT_CALL_TEST(test_avl_findGreaterOrEqual);
    CUT_CALL_TEST(test_avl_findLessOrEqual);
    CUT_CALL_TEST(test_avl_findIf);

    CUT_CALL_TEST(test_avl_remove);
    CUT_CALL_TEST(test_avl_remove_reverse);
    CUT_CALL_TEST(test_avl_remove_random);

    CUT_CALL_TEST(test_avl_isBalanced);
CUT_END_MAIN
