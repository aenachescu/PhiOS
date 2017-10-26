#ifndef PhiOS_DATA_STRUCTURES_AVL
#define PhiOS_DATA_STRUCTURES_AVL

#include "include/types.h"
#include "include/errors.h"
#include "include/preprocessor.h"

// uint32 copyType(type* dest, type* src, size_t size)
#ifndef AVL_COPY_TYPE_FUNC
#error "copy function is mandatory"
#endif

#ifndef AVL_FREE_TYPE_FUNC
#define AVL_FREE_TYPE_FUNC(x)
#endif

#define AVLStruct(name) CONCAT(name, AVL)
#define AVLFunc(name, func) CONCAT(AVLStruct(name), CONCAT(_, func))

#define AVLNodeStruct(name) CONCAT(name, AVLNode)
#define AVLNodeFunc(name, func) CONCAT(AVLNodeStruct(name), CONCAT(_, func))

#define DECLARE_AVL_TYPE(type, name)                                            \
    DECLARE_AVL_NODE_STRUCT(type, name)                                         \
    DECLARE_AVL_STRUCT(type, name)                                              \
    DECLARE_AVL_FUNC_INIT(type, name)                                           \
    DECLARE_AVL_FUNC_FREE(type, name)                                           \
    DECLARE_AVL_NODE_FUNC_INIT(type, name)                                      \
    DECLARE_AVL_NODE_FUNC_CREATE(type, name)

#define IMPLEMENT_AVL_TYPE(type, name)                                          \
    IMPLEMENT_AVL_FUNC_INIT(type, name)                                         \
    IMPLEMENT_AVL_FUNC_FREE(type, name)                                         \
    IMPLEMENT_AVL_NODE_FUNC_INIT(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_CREATE(type, name)

// declare AVLNodeStruct
#define DECLARE_AVL_NODE_STRUCT(type, name)                                     \
typedef struct CONCAT(AVLNodeStruct(name), _t)                                  \
{                                                                               \
    struct CONCAT(AVLNodeStruct(name), _t) *left;                               \
    struct CONCAT(AVLNodeStruct(name), _t) *right;                              \
    uint32 height;                                                              \
    type data;                                                                  \
} AVLNodeStruct(name);                                                          \

// declare AVLStruct
#define DECLARE_AVL_STRUCT(type, name)                                          \
typedef struct CONCAT(AVLStruct(name), _t)                                      \
{                                                                               \
    AVLNodeStruct(name) *root;                                                  \
} AVLStruct(name);                                                              \

// declare & implement INIT function for avl node
#define DECLARE_AVL_NODE_FUNC_INIT(type, name)                                  \
uint32 AVLNodeFunc(name, init) (                                                \
    AVLNodeStruct(name) *a_node,                                                \
    type *a_data                                                                \
);

#define IMPLEMENT_AVL_NODE_FUNC_INIT(type, name)                                \
uint32 AVLNodeFunc(name, init) (                                                \
    AVLNodeStruct(name) *a_node,                                                \
    type *a_data)                                                               \
{                                                                               \
    if (a_node == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_data == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    a_node->left = NULL;                                                        \
    a_node->right = NULL;                                                       \
    a_node->height = 1;                                                         \
                                                                                \
    uint32 err = AVL_COPY_TYPE_FUNC(&a_node->data, a_data, sizeof(type));       \
    if (err != ERROR_SUCCESS) {                                                 \
        return err;                                                             \
    }                                                                           \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

// if AVL_ALLOC_FUNC is defined the declare & implement CREATE function
// for avl node
#ifdef AVL_ALLOC_FUNC

#define DECLARE_AVL_NODE_FUNC_CREATE(type, name)                                \
uint32 AVLNodeFunc(name, create) (                                              \
    AVLNodeStruct(name) **a_node,                                               \
    type *a_data                                                                \
);

#define IMPLEMENT_AVL_NODE_FUNC_CREATE(type, name)                              \
uint32 AVLNodeFunc(name, create) (                                              \
    AVLNodeStruct(name) **a_node,                                               \
    type *a_data)                                                               \
{                                                                               \
    if (a_node == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_data == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    *a_node = (AVLNodeStruct(name)*) AVL_ALLOC_FUNC(sizeof(*a_node));           \
    if (a_node == NULL) {                                                       \
        return ERROR_NO_FREE_MEMORY;                                            \
    }                                                                           \
                                                                                \
    (*a_node)->left = NULL;                                                     \
    (*a_node)->right = NULL;                                                    \
    (*a_node)->height = 1;                                                      \
                                                                                \
    uint32 err = AVL_COPY_TYPE_FUNC(&(*a_node)->data, a_data, sizeof(type));    \
    if (err != ERROR_SUCCESS) {                                                 \
        return err;                                                             \
    }                                                                           \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#else

#define DECLARE_AVL_NODE_FUNC_CREATE(type, name)
#define IMPLEMENT_AVL_NODE_FUNC_CREATE(type, name)

#endif

// declare & implement INIT function
#define DECLARE_AVL_FUNC_INIT(type, name)                                       \
uint32 AVLFunc(name, init) (                                                    \
    AVLStruct(name) *a_avl                                                      \
);

#define IMPLEMENT_AVL_FUNC_INIT(type, name)                                     \
uint32 AVLFunc(name, init) (                                                    \
    AVLStruct(name) *a_avl)                                                     \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    a_avl->root = NULL;                                                         \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

// if AVL_FREE_FUNC is defined then declare & implement FREE function
#ifdef AVL_FREE_FUNC

#define DECLARE_AVL_FUNC_FREE(type, name)                                       \
uint32 AVLNodeFunc(name, free) (                                                \
    AVLNodeStruct(name) *a_node                                                 \
);                                                                              \
                                                                                \
uint32 AVLFunc(name, free) (                                                    \
    AVLStruct(name) *a_avl                                                      \
);

#define IMPLEMENT_AVL_FUNC_FREE(type, name)                                     \
uint32 AVLNodeFunc(name, free) (                                                \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    if (a_node == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    AVLNodeFunc(name, free)(a_node->left);                                      \
    AVLNodeFunc(name, free)(a_node->right);                                     \
                                                                                \
    AVL_FREE_TYPE_FUNC(a_node->data);                                           \
                                                                                \
    AVL_FREE_FUNC(a_node);                                                      \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}                                                                               \
                                                                                \
uint32 AVLFunc(name, free) (                                                    \
    AVLStruct(name) *a_avl)                                                     \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_avl->root == NULL) {                                                  \
        return ERROR_UNINITIALIZED;                                             \
    }                                                                           \
                                                                                \
    AVLNodeFunc(name, free)(a_avl->root);                                       \
    a_avl->root = NULL;                                                         \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#else

#define DECLARE_AVL_FUNC_FREE(type, name)
#define IMPLEMENT_AVL_FUNC_FREE(type, name)

#endif

#endif
