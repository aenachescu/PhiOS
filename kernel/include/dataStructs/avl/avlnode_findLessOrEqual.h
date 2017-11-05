#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_LESS_OR_EQUAL
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_LESS_OR_EQUAL

#ifdef AVL_USE_FIND_LESS_OR_EQUAL

// this is a data structure (or primitive data type) where the score between
// the element being searched and another element it's stored.
#ifndef AVL_TYPE_SCORE_LESS_OR_EQUAL
#error "type score less or equal undefined"
#endif

// clib_bool_t scoreIsInvalid(const ScoreType *a_score)
// return CLIB_TRUE if a_score is invalid
#ifndef AVL_TYPE_SCORE_LESS_OR_EQUAL_IS_INVALID
#error "score less or equal is invalid undefined"
#endif

// clib_bool_t scoreIsPerfect(const ScoreType *a_score)
// return CLIB_TRUE if a_score is perfect
#ifndef AVL_TYPE_SCORE_LESS_OR_EQUAL_IS_PERFECT
#error "score less or equal is perfect undefined"
#endif

// clib_bool_t scoreIsBetter(const ScoreType *a, const ScoreType *b)
// return CLIB_TRUE if a is better than b
#ifndef AVL_TYPE_SCORE_LESS_OR_EQUAL_IS_BETTER
#error "score less or equal is better undefined"
#endif

// void getScore(const Type *a_current, const Type *a_value, ScoreType *a_res)
// a_current - the node for which it's computed the score
// a_value - the value you are looking for
// a_res - the score for a_current
#ifndef AVL_TYPE_SCORE_LESS_OR_EQUAL_GET_SCORE
#error "score less or equal get score undefined"
#endif

// void scoreCopy(ScoreType *a_dst, const ScoreType *a_src)
#ifndef AVL_TYPE_SCORE_LESS_OR_EQUAL_COPY
#error "score less or equal copy undefined"
#endif

#define IMPLEMENT_AVL_NODE_FUNC_FIND_LESS_OR_EQUAL(type, name)                  \
static const AVLNodeStruct(name)* AVLNodeFunc(name, findLessOrEqual) (          \
    const AVLNodeStruct(name) *a_parent,                                        \
    const type *a_value,                                                        \
    AVL_TYPE_SCORE_LESS_OR_EQUAL *a_currentScore)                               \
{                                                                               \
    /*************************************************************************/ \
    /* if the score is perfect then return this node.                        */ \
    /*************************************************************************/ \
    if (AVL_TYPE_SCORE_LESS_OR_EQUAL_IS_PERFECT(a_currentScore)) {              \
        return a_parent;                                                        \
    }                                                                           \
                                                                                \
    /*************************************************************************/ \
    /* if the current node has an invalid score:                             */ \
    /*    - if the left child is null then return null, means we do not      */ \
    /*      have a node that matches the demand.                             */ \
    /*    - if the left child is not null then we continue to look on the    */ \
    /*      left branch for a node that has the best score or perfect score. */ \
    /*************************************************************************/ \
    if (AVL_TYPE_SCORE_LESS_OR_EQUAL_IS_INVALID(a_currentScore)) {              \
        if (a_parent->left == CLIB_NULLPTR) {                                   \
            return CLIB_NULLPTR;                                                \
        }                                                                       \
                                                                                \
        AVL_TYPE_SCORE_LESS_OR_EQUAL_GET_SCORE(                                 \
            (&a_parent->left->data),                                            \
            a_value,                                                            \
            a_currentScore                                                      \
        );                                                                      \
                                                                                \
        return AVLNodeFunc(name, findLessOrEqual) (                             \
            a_parent->left,                                                     \
            a_value,                                                            \
            a_currentScore                                                      \
        );                                                                      \
    }                                                                           \
                                                                                \
    /*************************************************************************/ \
    /* if the current node has an valid score:                               */ \
    /*    - if the right child is null then return a_parent because means    */ \
    /*      that is the best score.                                          */ \
    /*    - otherwise, we continue to look on the right branch for a node    */ \
    /*      that has the best score or the perfect score.                    */ \
    /*************************************************************************/ \
    if (a_parent->right == CLIB_NULLPTR) {                                      \
        return a_parent;                                                        \
    }                                                                           \
                                                                                \
    /*************************************************************************/ \
    /* get the score for the right child and then get the node with the best */ \
    /* score from the right.                                                 */ \
    /*************************************************************************/ \
    AVL_TYPE_SCORE_LESS_OR_EQUAL scoreRight;                                    \
    AVL_TYPE_SCORE_LESS_OR_EQUAL_GET_SCORE(                                     \
        (&a_parent->right->data),                                               \
        a_value,                                                                \
        (&scoreRight)                                                           \
    );                                                                          \
                                                                                \
    const AVLNodeStruct(name) *result =                                         \
        AVLNodeFunc(name, findLessOrEqual) (                                    \
            a_parent->right, a_value, (&scoreRight)                             \
        );                                                                      \
                                                                                \
    /*************************************************************************/ \
    /* if the node with the best score from the right branch has an score    */ \
    /* better than the score of the current node, then copy the score in the */ \
    /* a_currentScore parameter and return the node from the right branch,   */ \
    /* otherwise return the current node.                                    */ \
    /*************************************************************************/ \
    if (AVL_TYPE_SCORE_LESS_OR_EQUAL_IS_BETTER(                                 \
            (&scoreRight), a_currentScore)) {                                   \
                                                                                \
        AVL_TYPE_SCORE_LESS_OR_EQUAL_COPY(a_currentScore, (&scoreRight));       \
                                                                                \
        return result;                                                          \
    }                                                                           \
                                                                                \
    return a_parent;                                                            \
}

#else // AVL_USE_FIND_LESS_OR_EQUAL is not defined

#define IMPLEMENT_AVL_NODE_FUNC_FIND_LESS_OR_EQUAL(type, name)

#endif // end if AVL_USE_FIND_LESS_OR_EQUAL

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_LESS_OR_EQUAL
