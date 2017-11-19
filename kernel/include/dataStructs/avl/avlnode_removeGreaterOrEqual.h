#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_GREATER_OR_EQUAL
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_GREATER_OR_EQUAL

#ifdef AVL_USE_REMOVE_GREATER_OR_EQUAL

// this is a data structure (or primitive data type) where the score between
// the element being searched and another element it's stored.
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL
#error "type score greater or equal undefined"
#endif

// clib_bool_t scoreIsInvalid(const ScoreType *a_score)
// return CLIB_TRUE if a_score is invalid
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_INVALID
#error "score greater or equal is invalid undefined"
#endif

// clib_bool_t scoreIsPerfect(const ScoreType *a_score)
// return CLIB_TRUE if a_score is perfect
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_PERFECT
#error "score greater or equal is perfect undefined"
#endif

// clib_bool_t scoreIsBetter(const ScoreType *a, const ScoreType *b)
// return CLIB_TRUE if a is better than b
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

#define IMPLEMENT_AVL_NODE_FUNC_REMOVE_GREATER_OR_EQUAL(type, name)             \
static AVLNodeStruct(name)* AVLNodeFunc(name, removeGreaterOrEqual) (           \
    AVLNodeStruct(name) *a_parent,                                              \
    const type *a_value,                                                        \
    AVLNodeStruct(name) **a_res)                                                \
{                                                                               \
    AVL_TYPE_SCORE_GREATER_OR_EQUAL currentScore;                               \
    AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE(                                  \
        (&a_parent->data),                                                      \
        a_value,                                                                \
        (&currentScore)                                                         \
    );                                                                          \
                                                                                \
    if (AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_PERFECT((&currentScore))) {          \
        goto removeNode;                                                        \
    }                                                                           \
                                                                                \
    if (AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_INVALID((&currentScore))) {          \
        if (a_parent->right == CLIB_NULLPTR) {                                  \
            return a_parent;                                                    \
        }                                                                       \
                                                                                \
        a_parent->right = AVLNodeFunc(name, removeGreaterOrEqual) (             \
            a_parent->right,                                                    \
            a_value,                                                            \
            a_res                                                               \
        );                                                                      \
                                                                                \
        if (*a_res == CLIB_NULLPTR) {                                           \
            return a_parent;                                                    \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, balance) (a_parent);                           \
    }                                                                           \
                                                                                \
    if (a_parent->left == CLIB_NULLPTR) {                                       \
        goto removeNode;                                                        \
    }                                                                           \
                                                                                \
    a_parent->left = AVLNodeFunc(name, removeGreaterOrEqual) (                  \
        a_parent->left,                                                         \
        a_value,                                                                \
        a_res                                                                   \
    );                                                                          \
                                                                                \
    if (*a_res == CLIB_NULLPTR) {                                               \
        goto removeNode;                                                        \
    }                                                                           \
                                                                                \
    return AVLNodeFunc(name, balance) (a_parent);                               \
                                                                                \
removeNode:                                                                     \
    *a_res = a_parent;                                                          \
                                                                                \
    AVLNodeStruct(name) *auxNode;                                               \
                                                                                \
    if (a_parent->right == CLIB_NULLPTR) {                                      \
        auxNode = a_parent->left;                                               \
        a_parent->left = CLIB_NULLPTR;                                          \
        return auxNode;                                                         \
    }                                                                           \
                                                                                \
    AVLNodeStruct(name) *minNode = CLIB_NULLPTR;                                \
    auxNode = AVLNodeFunc(name, removeMin) (a_parent->right, &minNode);         \
                                                                                \
    minNode->right = auxNode;                                                   \
    minNode->left = a_parent->left;                                             \
                                                                                \
    a_parent->left = CLIB_NULLPTR;                                              \
    a_parent->right = CLIB_NULLPTR;                                             \
                                                                                \
    return AVLNodeFunc(name, balance) (minNode);                                \
}

#else // AVL_USE_REMOVE_GREATER_OR_EQUAL is not defined

#define IMPLEMENT_AVL_NODE_FUNC_REMOVE_GREATER_OR_EQUAL(type, name)

#endif // end if AVL_USE_REMOVE_GREATER_OR_EQUAL

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_GREATER_OR_EQUAL
