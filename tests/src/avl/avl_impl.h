#ifndef PhiOS_UNIT_TESTING_AVL_IMPL
#define PhiOS_UNIT_TESTING_AVL_IMPL

#include <stdlib.h>

// defines for findGreaterOrEqual
#define AVL_USE_FIND_GREATER_OR_EQUAL
#define AVL_TYPE_SCORE_GREATER_OR_EQUAL unsigned int

#define AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_INVALID(x)                           \
    ((*x) == 0xFFFFFFFF)

#define AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_PERFECT(x)                           \
    ((*x) == 0)

#define AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_BETTER(a, b)                         \
    ((*a) < (*b))

#define AVL_TYPE_SCORE_GREATER_OR_EQUAL_COPY(a, b)                              \
    ((*a) = (*b))

#define AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE(current, value, result)       \
    current->data >= value->data ?                                              \
        (*result = current->data - value->data) : (*result = 0xFFFFFFFF)

#define AVL_USE_AVL_NODE_FREE
#define AVL_USE_AVL_NODE_CREATE

// basic defines
#define CLIB_NULLPTR                        NULL
#define AVL_DESTROY_TYPE_FUNC(x)            AVLDestroyData(x)
#define AVL_FREE_FUNC(x)                    AVLFreeNode(x)
#define AVL_ALLOC_FUNC(x)                   AVLAllocNode(x)
#define AVL_COPY_TYPE_FUNC(dest, src)       AVLCopyData(dest, src)
#define AVL_TYPE_OPERATOR_GREATER(a, b)     (a->data > b->data)
#define AVL_TYPE_OPERATOR_LESS(a, b)        (a->data < b->data)

#include "kernel/include/dataStructs/avl/avl.h"

typedef struct _Data
{
    unsigned int data;
} Data;

void AVLAllocNodeSetError();
void* AVLAllocNode(size_t a_size);
void AVLFreeNode(void *a_node);

void AVLDestroyData(Data *a_data);

void AVLCopyDataSetError();
avl_error_code_t AVLCopyData(Data *dest, const Data *src);

size_t GetMemoryInUsage();
void ResetMemoryInUsage();

size_t GetObjectsInUsage();
void ResetObjectsInUsage();

size_t GetAllocCalls();
void ResetAllocCalls();

size_t GetFreeCalls();
void ResetFreeCalls();

DECLARE_AVL_TYPE(Data, UTData);

#endif
