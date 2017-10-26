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
    DECLARE_AVL_NODE_FUNC_INIT(type, name)                                      \
    DECLARE_AVL_NODE_FUNC_CREATE(type, name)                                    \
    DECLARE_AVL_FUNC_INIT(type, name)                                           \
    DECLARE_AVL_FUNC_FREE(type, name)

#define IMPLEMENT_AVL_TYPE(type, name)                                          \
    IMPLEMENT_AVL_NODE_HELPERS(type, name)                                      \
    IMPLEMENT_AVL_NODE_FUNC_ROTATE_LEFT(type, name)                             \
    IMPLEMENT_AVL_NODE_FUNC_ROTATE_RIGHT(type, name)                            \
    IMPLEMENT_AVL_NODE_FUNC_BALANCE(type, name)                                 \
    IMPLEMENT_AVL_NODE_FUNC_INIT(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_CREATE(type, name)                                  \
    IMPLEMENT_AVL_FUNC_INIT(type, name)                                         \
    IMPLEMENT_AVL_FUNC_FREE(type, name)

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

// implement avl node helper functions
#define IMPLEMENT_AVL_NODE_HELPERS(type, name)                                  \
static inline uint32 AVLNodeFunc(name, getHeight) (                             \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    return a_node ? a_node->height : 0;                                         \
}                                                                               \
                                                                                \
static inline uint32 AVLNodeFunc(name, getMaxHeight) (                          \
    AVLNodeStruct(name) *a_node1,                                               \
    AVLNodeStruct(name) *a_node2)                                               \
{                                                                               \
    uint32 height1 = AVLNodeFunc(name, getHeight) (a_node1);                    \
    uint32 height2 = AVLNodeFunc(name, getHeight) (a_node2);                    \
                                                                                \
    return height1 > height2 ? height1 : height2;                               \
}                                                                               \
                                                                                \
static inline void AVLNodeFunc(name, calculateHeight) (                         \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    a_node->height =                                                            \
        1 + AVLNodeFunc(name, getMaxHeight) (a_node->left, a_node->right);      \
}

// implement avl node rotate left
#define IMPLEMENT_AVL_NODE_FUNC_ROTATE_LEFT(type, name)                         \
static AVLNodeStruct(name)* AVLNodeFunc(name, rotateLeft) (                     \
    AVLNodeStruct(name) *a_parent)                                              \
{                                                                               \
    AVLNodeStruct(name) *newParent = a_parent->right;                           \
                                                                                \
    a_parent->right = newParent->left;                                          \
    newParent->left = a_parent;                                                 \
                                                                                \
    AVLNodeFunc(name, calculateHeight) (a_parent);                              \
    AVLNodeFunc(name, calculateHeight) (newParent);                             \
                                                                                \
    return newParent;                                                           \
}

// implement avl node rotate right
#define IMPLEMENT_AVL_NODE_FUNC_ROTATE_RIGHT(type, name)                        \
static AVLNodeStruct(name)* AVLNodeFunc(name, rotateRight) (                    \
    AVLNodeStruct(name) *a_parent)                                              \
{                                                                               \
    AVLNodeStruct(name) *newParent = a_parent->left;                            \
                                                                                \
    a_parent->left = newParent->right;                                          \
    newParent->right = a_parent;                                                \
                                                                                \
    AVLNodeFunc(name, calculateHeight) (a_parent);                              \
    AVLNodeFunc(name, calculateHeight) (newParent);                             \
                                                                                \
    return newParent;                                                           \
}

// implement avl node balance
#define GET_HEIGHT(x) AVLNodeFunc(name, getHeight) (x)
#define IMPLEMENT_AVL_NODE_FUNC_BALANCE(type, name)                             \
static AVLNodeStruct(name)* AVLNodeFunc(name, balance) (                        \
    AVLNodeStruct(name) *a_parent)                                              \
{                                                                               \
    AVLNodeFunc(name, calculateHeight) (a_parent);                              \
                                                                                \
    if (GET_HEIGHT(a_parent->left) - GET_HEIGHT(a_parent->right) == 2) {        \
        if (GET_HEIGHT(a_parent->left->right) >                                 \
            GET_HEIGHT(a_parent->left->left)) {                                 \
            a_parent->left = AVLNodeFunc(name, rotateLeft) (a_parent->left);    \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, rotateRight) (a_parent);                       \
    }                                                                           \
                                                                                \
    if (GET_HEIGHT(a_parent->right) - GET_HEIGHT(a_parent->left) == 2) {        \
        if (GET_HEIGHT(a_parent->right->left) >                                 \
            GET_HEIGHT(a_parent->right->right)) {                               \
            a_parent->right = AVLNodeFunc(name, rotateRight) (a_parent->right)  \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, rotateLeft) (a_parent);                        \
    }                                                                           \
                                                                                \
    return a_parent;                                                            \
}
#undef GET_HEIGHT

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
