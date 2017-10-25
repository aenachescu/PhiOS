#ifndef PhiOS_DATA_STRUCTURES_AVL
#define PhiOS_DATA_STRUCTURES_AVL

#include "include/types.h"
#include "include/errors.h"
#include "include/preprocessor.h"

typedef void* (*PfnAlloc)(size_t size);

#ifdef AVL_USE_PFN_ALLOC

#define AVL_PFN_ALLOC(x) PfnAlloc x;
#define AVL_PFN_ALLOC_DECLARE_PARAM(x) , PfnAlloc x
#define AVL_PFN_ALLOC_USE_PARAM(x) x
#define AVL_SET_ALLOC_FN(avl, value) avl->allocFn = value
#define AVL_CHECK_IF_ALLOC_FN_IS_NOT_NULL(allocFn) (allocFn != NULL)
#define AVL_CHECK_IF_ALLOC_FN_IS_NULL(allocFn) (allocFn == NULL)

#else

#define AVL_PFN_ALLOC(x)
#define AVL_PFN_ALLOC_DECLARE_PARAM(x)
#define AVL_PFN_ALLOC_USE_PARAM(x)
#define AVL_SET_ALLOC_FN(avl, allocFn)
#define AVL_CHECK_IF_ALLOC_FN_IS_NOT_NULL(allocFn) (true)
#define AVL_CHECK_IF_ALLOC_FN_IS_NULL(allocFn) (false)

#endif

#define AVLStruct(name) CONCAT(name, AVL)
#define AVLFunc(name, func) CONCAT(AVLStruct(name), CONCAT(_, func))

#define DECLARE_AVL_TYPE(type, name)                                            \
typedef struct CONCAT(CONCAT(name, AVLNode), _t)                                \
{                                                                               \
    struct CONCAT(CONCAT(name, AVLNode), _t) *left;                             \
    struct CONCAT(CONCAT(name, AVLNode), _t) *right;                            \
    type data;                                                                  \
} CONCAT(name, AVLNode);                                                        \
                                                                                \
typedef struct CONCAT(CONCAT(name, AVL), _t)                                    \
{                                                                               \
    CONCAT(name, AVLNode) *root;                                                \
    AVL_PFN_ALLOC(allocFn)                                                      \
} AVLStruct(name);                                                              \
                                                                                \
uint32 AVLFunc(name, init) (                                                    \
    AVLStruct(name) *a_avl                                                      \
    AVL_PFN_ALLOC_DECLARE_PARAM(a_allocFn)                                      \
);

#define IMPLEMENT_AVL_TYPE(type, name)                                          \
uint32 AVLFunc(name, init) (                                                    \
    AVLStruct(name) *a_avl                                                      \
    AVL_PFN_ALLOC_DECLARE_PARAM(a_allocFn))                                     \
{                                                                               \
    if (a_avl != NULL &&                                                        \
        AVL_CHECK_IF_ALLOC_FN_IS_NULL(a_allocFn)) {                             \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    a_avl->root = NULL;                                                         \
    AVL_SET_ALLOC_FN(a_avl, a_allocFn);                                         \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#endif
