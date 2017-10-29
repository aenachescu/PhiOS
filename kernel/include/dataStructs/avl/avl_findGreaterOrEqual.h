#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_GREATER_OR_EQUAL
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_GREATER_OR_EQUAL

#ifdef AVL_USE_FIND_GREATER_OR_EQUAL

// void getScore(const Type *a_current, const Type *a_value, ScoreType *a_res)
// a_current - the node for which it's computed the score
// a_value - the value you are looking for
// a_res - the score for a_current
#ifndef AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE
#error "score greater or equal get score undefined"
#endif

#define DECLARE_AVL_FUNC_FIND_GREATER_OR_EQUAL(type, name)                      \
uint32 AVLFunc(name, findGreaterOrEqual) (                                      \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const AVLNodeStruct(name) **a_res                                           \
);

#define IMPLEMENT_AVL_FUNC_FIND_GREATER_OR_EQUAL(type, name)                    \
uint32 AVLFunc(name, findGreaterOrEqual) (                                      \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const AVLNodeStruct(name) **a_res)                                          \
{                                                                               \
    if (a_res == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    *a_res = NULL;                                                              \
                                                                                \
    if (a_avl == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_value == NULL) {                                                      \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_avl->root == NULL) {                                                  \
        return ERROR_NOT_FOUND;                                                 \
    }                                                                           \
                                                                                \
    AVL_TYPE_SCORE_GREATER_OR_EQUAL score;                                      \
    AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE(                                  \
        (&a_avl->root->data),                                                   \
        a_value,                                                                \
        (&score)                                                                \
    );                                                                          \
                                                                                \
    *a_res = AVLNodeFunc(name, findGreaterOrEqual) (                            \
        a_avl->root,                                                            \
        a_value,                                                                \
        &score                                                                  \
    );                                                                          \
                                                                                \
    if (*a_res == NULL) {                                                       \
        return ERROR_NOT_FOUND;                                                 \
    }                                                                           \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#else // AVL_USE_FIND_GREATER_OR_EQUAL is not defined

#define IMPLEMENT_AVL_FUNC_FIND_GREATER_OR_EQUAL(type, name)

#endif // end if AVL_USE_FIND_GREATER_OR_EQUAL

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_GREATER_OR_EQUAL
