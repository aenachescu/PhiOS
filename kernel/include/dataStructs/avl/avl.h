#ifndef PhiOS_DATA_STRUCTURES_AVL
#define PhiOS_DATA_STRUCTURES_AVL

#include "include/types.h"
#include "include/errors.h"
#include "include/preprocessor.h"

// uint32 copyType(type* dest, type* src, size_t size)
#ifndef AVL_COPY_TYPE_FUNC
#error "copy function is mandatory"
#endif

// bool opEqual(type* a, type* b)
#ifndef AVL_TYPE_OPERATOR_EQUAL
#error "operator== is mandatory"
#endif

// bool opGreater(type* a, type* b)
#ifndef AVL_TYPE_OPERATOR_GREATER
#error "operator> is mandatory "
#endif

// bool opLess(type* a, type* b)
#ifndef AVL_TYPE_OPERATOR_LESS
#error "operator< is mandatory"
#endif

#ifdef AVL_USE_FIND_GREATER_OR_EQUAL

// that is an structure for the score between two elements from avl
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL
#error "type score greater or equal undefined"
#endif

// bool scoreIsInvalid(const ScoreType *a_score)
// return true if a_score is invalid
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_INVALID
#error "score greater or equal is invalid undefined"
#endif

// bool scoreIsPerfect(const ScoreType *a_score)
// return true if a_score is perfect
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_PERFECT
#error "score greater or equal is perfect undefined"
#endif

// bool scoreIsBetter(const ScoreType *a, const ScoreType *b)
// return true if a is better than b
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_BETTER
#error "score greater or equal is better undefined"
#endif

// void getScore(const Type *a_current, const Type *a_value, ScoreType *a_res)
// a_current - the node for which it's computed the score
// a_value - the value you are looking for
// a_res - the score for a_current
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE
#error "score greater or equal get score undefined"
#endif

// void scoreCopy(ScoreType *a_dst, const ScoreType *a_src)
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL_COPY
#error "score greater or equal copy undefined"
#endif

#endif // end if AVL_USE_FIND_GREATER_OR_EQUAL

#ifndef AVL_FREE_TYPE_FUNC
#define AVL_FREE_TYPE_FUNC(x)
#endif

#define AVLStruct(name) CONCAT(name, AVL)
#define AVLFunc(name, func) CONCAT(AVLStruct(name), CONCAT(_, func))

#define AVLNodeStruct(name) CONCAT(name, AVLNode)
#define AVLNodeFunc(name, func) CONCAT(AVLNodeStruct(name), CONCAT(_, func))

// declare avl type
#define DECLARE_AVL_STRUCTS(type, name)                                         \
    DECLARE_AVL_NODE_STRUCT(type, name)                                         \
    DECLARE_AVL_STRUCT(type, name)

#define DECLARE_AVL_NODE_FUNC(type, name)                                       \
    DECLARE_AVL_NODE_FUNC_INIT(type, name)                                      \
    DECLARE_AVL_NODE_FUNC_CREATE(type, name)

#define DECLARE_AVL_FUNC(type, name)                                            \
    DECLARE_AVL_FUNC_INIT(type, name)                                           \
    DECLARE_AVL_FUNC_FREE(type, name)                                           \
    DECLARE_AVL_FUNC_GET_HEIGHT(type, name)                                     \
    DECLARE_AVL_FUNC_INSERT_NODE(type, name)                                    \
    DECLARE_AVL_FUNC_INSERT(type, name)                                         \
    DECLARE_AVL_FUNC_FIND(type, name)                                           \
    DECLARE_AVL_FUNC_FIND_TYPE(type, name)

#define DECLARE_AVL_TYPE(type, name)                                            \
    DECLARE_AVL_STRUCTS(type, name)                                             \
    DECLARE_AVL_NODE_FUNC(type, name)                                           \
    DECLARE_AVL_FUNC(type, name)

// implement avl type
#define IMPLEMENT_AVL_NODE_FUNC(type, name)                                     \
    IMPLEMENT_AVL_NODE_HELPERS(type, name)                                      \
    IMPLEMENT_AVL_NODE_FUNC_ROTATE_LEFT(type, name)                             \
    IMPLEMENT_AVL_NODE_FUNC_ROTATE_RIGHT(type, name)                            \
    IMPLEMENT_AVL_NODE_FUNC_BALANCE(type, name)                                 \
    IMPLEMENT_AVL_NODE_FUNC_INSERT(type, name)                                  \
    IMPLEMENT_AVL_NODE_FUNC_INIT(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_CREATE(type, name)                                  \
    IMPLEMENT_AVL_NODE_FUNC_FIND(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_FIND_GREATER_OR_EQUAL(type, name)

#define IMPLEMENT_AVL_FUNC(type, name)                                          \
    IMPLEMENT_AVL_FUNC_INIT(type, name)                                         \
    IMPLEMENT_AVL_FUNC_FREE(type, name)                                         \
    IMPLEMENT_AVL_FUNC_GET_HEIGHT(type, name)                                   \
    IMPLEMENT_AVL_FUNC_INSERT_NODE(type, name)                                  \
    IMPLEMENT_AVL_FUNC_INSERT(type, name)                                       \
    IMPLEMENT_AVL_FUNC_FIND(type, name)                                         \
    IMPLEMENT_AVL_FUNC_FIND_TYPE(type, name)

#define IMPLEMENT_AVL_TYPE(type, name)                                          \
    IMPLEMENT_AVL_NODE_FUNC(type, name)                                         \
    IMPLEMENT_AVL_FUNC(type, name)

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
    const AVLNodeStruct(name) *a_node)                                          \
{                                                                               \
    return a_node ? a_node->height : 0;                                         \
}                                                                               \
                                                                                \
static inline uint32 AVLNodeFunc(name, getMaxHeight) (                          \
    const AVLNodeStruct(name) *a_node1,                                         \
    const AVLNodeStruct(name) *a_node2)                                         \
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
}                                                                               \
                                                                                \
static inline bool AVLNodeFunc(name, isHeightGreater) (                         \
    const AVLNodeStruct(name) *a_node1,                                         \
    const AVLNodeStruct(name) *a_node2)                                         \
{                                                                               \
    if (AVLNodeFunc(name, getHeight) (a_node1) >                                \
        AVLNodeFunc(name, getHeight) (a_node2)) {                               \
        return true;                                                            \
    }                                                                           \
                                                                                \
    return false;                                                               \
}                                                                               \
                                                                                \
static inline uint32 AVLNodeFunc(name, diffHeight) (                            \
    AVLNodeStruct(name) *a_node1,                                               \
    AVLNodeStruct(name) *a_node2)                                               \
{                                                                               \
    uint32 height1 = AVLNodeFunc(name, getHeight) (a_node1);                    \
    uint32 height2 = AVLNodeFunc(name, getHeight) (a_node2);                    \
                                                                                \
    if (height1 > height2) {                                                    \
        return height1 - height2;                                               \
    }                                                                           \
                                                                                \
    return 0;                                                                   \
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
#define IMPLEMENT_AVL_NODE_FUNC_BALANCE(type, name)                             \
static AVLNodeStruct(name)* AVLNodeFunc(name, balance) (                        \
    AVLNodeStruct(name) *a_parent)                                              \
{                                                                               \
    AVLNodeFunc(name, calculateHeight) (a_parent);                              \
                                                                                \
    if (AVLNodeFunc(name, diffHeight) (a_parent->left, a_parent->right) == 2) { \
        if (AVLNodeFunc(name, isHeightGreater) (                                \
                a_parent->left->right, a_parent->left->left)) {                 \
            a_parent->left = AVLNodeFunc(name, rotateLeft) (a_parent->left);    \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, rotateRight) (a_parent);                       \
    }                                                                           \
                                                                                \
    if (AVLNodeFunc(name, diffHeight) (a_parent->right, a_parent->left) == 2) { \
        if (AVLNodeFunc(name, isHeightGreater) (                                \
                a_parent->right->left, a_parent->right->right)) {               \
            a_parent->right = AVLNodeFunc(name, rotateRight) (a_parent->right); \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, rotateLeft) (a_parent);                        \
    }                                                                           \
                                                                                \
    return a_parent;                                                            \
}

// implement avl node insert
#define IMPLEMENT_AVL_NODE_FUNC_INSERT(type, name)                              \
static AVLNodeStruct(name)* AVLNodeFunc(name, insert) (                         \
    AVLNodeStruct(name) *a_parent,                                              \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    if (a_parent == NULL) {                                                     \
        return a_node;                                                          \
    }                                                                           \
                                                                                \
    if (AVL_TYPE_OPERATOR_LESS((&a_node->data), (&a_parent->data))) {           \
        a_parent->left = AVLNodeFunc(name, insert) (a_parent->left, a_node);    \
    } else {                                                                    \
        a_parent->right = AVLNodeFunc(name, insert) (a_parent->right, a_node);  \
    }                                                                           \
                                                                                \
    return AVLNodeFunc(name, balance) (a_parent);                               \
}

// implement avl node find
#define IMPLEMENT_AVL_NODE_FUNC_FIND(type, name)                                \
static const AVLNodeStruct(name)* AVLNodeFunc(name, find) (                     \
    const AVLNodeStruct(name) *a_parent,                                        \
    const type *a_value)                                                        \
{                                                                               \
    if (a_parent == NULL) {                                                     \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    if (AVL_TYPE_OPERATOR_LESS(a_value, &a_parent->data)) {                     \
        return AVLNodeFunc(name, find) (a_parent->left, a_value);               \
    }                                                                           \
                                                                                \
    if (AVL_TYPE_OPERATOR_GREATER(a_value, &a_parent->data)) {                  \
        return AVLNodeFunc(name, find) (a_parent->right, a_value);              \
    }                                                                           \
                                                                                \
    return a_parent;                                                            \
}

// implement avl node find greater than or equal to
#ifdef AVL_USE_FIND_GREATER_OR_EQUAL

#define IMPLEMENT_AVL_NODE_FUNC_FIND_GREATER_OR_EQUAL(type, name)               \
static const AVLNodeStruct(name)* AVLNodeFunc(name, findGreaterOrEqual) (       \
    const AVLNodeStruct(name) *a_parent,                                        \
    const type *a_value,                                                        \
    AVL_TYPE_SCORE_GREATER_OR_EQUAL *a_currentScore)                            \
{                                                                               \
    /*************************************************************************/ \
    /* if the score is perfect then return this node.                        */ \
    /*************************************************************************/ \
    if (AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_PERFECT(a_currentScore)) {           \
        return a_parent;                                                        \
    }                                                                           \
                                                                                \
    /*************************************************************************/ \
    /* if the current node has an invalid score:                             */ \
    /*    - if the right child is null then return null, means we do not     */ \
    /*      have a node that matches the demand.                             */ \
    /*    - if the right child is not null then we continue to look on the   */ \
    /*      right branch for a node that has the best score or perfect score.*/ \
    /*************************************************************************/ \
    if (AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_INVALID(a_currentScore)) {           \
        if (a_parent->right == NULL) {                                          \
            return NULL;                                                        \
        }                                                                       \
                                                                                \
        AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE(                              \
            (&a_parent->right->data),                                           \
            a_value,                                                            \
            a_currentScore                                                      \
        );                                                                      \
                                                                                \
        return AVLNodeFunc(name, findGreaterOrEqual) (                          \
            a_parent->right,                                                    \
            a_value,                                                            \
            a_currentScore                                                      \
        );                                                                      \
    }                                                                           \
                                                                                \
    /*************************************************************************/ \
    /* if the current node has an valid score:                               */ \
    /*    - if the left child is null then return a_parent because means     */ \
    /*      that is the best score.                                          */ \
    /*    - otherwise, we continue to look on the left branch for a node that*/ \
    /*      has the best score or the perfect score.                         */ \
    /*************************************************************************/ \
    if (a_parent->left == NULL) {                                               \
        return a_parent;                                                        \
    }                                                                           \
                                                                                \
    /*************************************************************************/ \
    /* get the score for the left child and then get the node with the best  */ \
    /* score from the left.                                                  */ \
    /*************************************************************************/ \
    AVL_TYPE_SCORE_GREATER_OR_EQUAL scoreLeft;                                  \
    AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE(                                  \
        (&a_parent->left->data),                                                \
        a_value,                                                                \
        (&scoreLeft)                                                            \
    );                                                                          \
                                                                                \
    const AVLNodeStruct(name) *result =                                         \
        AVLNodeFunc(name, findGreaterOrEqual) (                                 \
            a_parent->left, a_value, (&scoreLeft)                               \
        );                                                                      \
                                                                                \
    /*************************************************************************/ \
    /* if the node with the best score from the left branch has an score     */ \
    /* better than the score of the current node, then copy the score in the */ \
    /* a_currentScore parameter and return the node from the left branch,    */ \
    /* otherwise return the current node.                                    */ \
    /*************************************************************************/ \
    if (AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_BETTER(                              \
            (&scoreLeft), a_currentScore)) {                                    \
                                                                                \
        AVL_TYPE_SCORE_GREATER_OR_EQUAL_COPY(a_currentScore, (&scoreLeft));     \
                                                                                \
        return result;                                                          \
    }                                                                           \
                                                                                \
    return a_parent;                                                            \
}

#else

#define IMPLEMENT_AVL_NODE_FUNC_FIND_GREATER_OR_EQUAL(type, name)

#endif

// declare & implement INIT function for avl node
#define DECLARE_AVL_NODE_FUNC_INIT(type, name)                                  \
uint32 AVLNodeFunc(name, init) (                                                \
    AVLNodeStruct(name) *a_node,                                                \
    const type *a_data                                                          \
);

#define IMPLEMENT_AVL_NODE_FUNC_INIT(type, name)                                \
uint32 AVLNodeFunc(name, init) (                                                \
    AVLNodeStruct(name) *a_node,                                                \
    const type *a_data)                                                         \
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
    uint32 err = AVL_COPY_TYPE_FUNC(&a_node->data, a_data);                     \
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
    const type *a_data                                                          \
);

#define IMPLEMENT_AVL_NODE_FUNC_CREATE(type, name)                              \
uint32 AVLNodeFunc(name, create) (                                              \
    AVLNodeStruct(name) **a_node,                                               \
    const type *a_data)                                                         \
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
    uint32 err = AVL_COPY_TYPE_FUNC(&(*a_node)->data, a_data);                  \
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
    AVL_FREE_TYPE_FUNC(&a_node->data);                                          \
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

// declare & implement avl get height
#define DECLARE_AVL_FUNC_GET_HEIGHT(type, name)                                 \
uint32 AVLFunc(name, getHeight) (                                               \
    const AVLStruct(name) *a_avl                                                \
);

#define IMPLEMENT_AVL_FUNC_GET_HEIGHT(type, name)                               \
uint32 AVLFunc(name, getHeight) (                                               \
    const AVLStruct(name) *a_avl)                                               \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return 0;                                                               \
    }                                                                           \
                                                                                \
    return AVLNodeFunc(name, getHeight) (a_avl->root);                          \
}

// declare & implement avl insertNode
#define DECLARE_AVL_FUNC_INSERT_NODE(type, name)                                \
uint32 AVLFunc(name, insertNode) (                                              \
    AVLStruct(name) *a_avl,                                                     \
    AVLNodeStruct(name) *a_node                                                 \
);

#define IMPLEMENT_AVL_FUNC_INSERT_NODE(type, name)                              \
uint32 AVLFunc(name, insertNode) (                                              \
    AVLStruct(name) *a_avl,                                                     \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_node == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    a_avl->root = AVLNodeFunc(name, insert) (a_avl->root, a_node);              \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

// if AVL_ALLOC_FUNC is defined then declare & implement avl insert
#ifdef AVL_ALLOC_FUNC

#define DECLARE_AVL_FUNC_INSERT(type, name)                                     \
uint32 AVLFunc(name, insert) (                                                  \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value                                                         \
);

#define IMPLEMENT_AVL_FUNC_INSERT(type, name)                                   \
uint32 AVLFunc(name, insert) (                                                  \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value)                                                        \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_value == NULL) {                                                      \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    AVLNodeStruct(name) *node = NULL;                                           \
    uint32 err = AVLNodeFunc(name, create) (&node, a_value);                    \
    if (err != ERROR_SUCCESS) {                                                 \
        return err;                                                             \
    }                                                                           \
                                                                                \
    a_avl->root = AVLNodeFunc(name, insert) (a_avl->root, node);                \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#else

#define DECLARE_AVL_FUNC_INSERT(type, name)
#define IMPLEMENT_AVL_FUNC_INSERT(type, name)

#endif

// declare & implement avl find
#define DECLARE_AVL_FUNC_FIND(type, name)                                       \
const AVLNodeStruct(name)* AVLFunc(name, find) (                                \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value                                                         \
);

#define IMPLEMENT_AVL_FUNC_FIND(type, name)                                     \
const AVLNodeStruct(name)* AVLFunc(name, find) (                                \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value)                                                        \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    if (a_value == NULL) {                                                      \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    return AVLNodeFunc(name, find) (a_avl->root, a_value);                      \
}

// declare & implement avl findType
#define DECLARE_AVL_FUNC_FIND_TYPE(type, name)                                  \
const type* AVLFunc(name, findType) (                                           \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value                                                         \
);

#define IMPLEMENT_AVL_FUNC_FIND_TYPE(type, name)                                \
const type* AVLFunc(name, findType) (                                           \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value)                                                        \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    if (a_value == NULL) {                                                      \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    const AVLNodeStruct(name) *node =                                           \
        AVLNodeFunc(name, find) (a_avl->root, a_value);                         \
                                                                                \
    if (node == NULL) {                                                         \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    return &node->data;                                                         \
}

#endif
