#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_NEAR_OR_EQUAL
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_NEAR_OR_EQUAL

#ifdef AVL_USE_FIND_NEAR_OR_EQUAL

// this is a data structure (or primitive data type) where the score between
// the element being searched and another element it's stored.
#ifndef AVL_TYPE_SCORE_NEAR_OR_EQUAL
#error "type score near or equal undefined"
#endif

// clib_bool_t scoreIsPerfect(const ScoreType *a_score)
// return CLIB_TRUE if a_score is perfect
#ifndef AVL_TYPE_SCORE_NEAR_OR_EQUAL_IS_PERFECT
#error "score near or equal is perfect undefined"
#endif

// clib_bool_t scoreIsBetter(const ScoreType *a, const ScoreType *b)
// return CLIB_TRUE if a is better than b
#ifndef AVL_TYPE_SCORE_NEAR_OR_EQUAL_IS_BETTER
#error "score near or equal is better undefined"
#endif

// void getScore(const Type *a_current, const Type *a_value, ScoreType *a_res)
// a_current - the node for which it's computed the score
// a_value - the value you are looking for
// a_res - the score for a_current
#ifndef AVL_TYPE_SCORE_NEAR_OR_EQUAL_GET_SCORE
#error "score near or equal get score undefined"
#endif

// void scoreCopy(ScoreType *a_dst, const ScoreType *a_src)
#ifndef AVL_TYPE_SCORE_NEAR_OR_EQUAL_COPY
#error "score near or equal copy undefined"
#endif
#include <stdio.h>
#define IMPLEMENT_AVL_NODE_FUNC_FIND_NEAR_OR_EQUAL(type, name)                  \
static const AVLNodeStruct(name)* AVLNodeFunc(name, findNearOrEqual) (          \
    const AVLNodeStruct(name) *a_parent,                                        \
    const type *a_value,                                                        \
    AVL_TYPE_SCORE_NEAR_OR_EQUAL *a_currentScore)                               \
{                                                                               \
    /*************************************************************************/ \
    /* if the score is perfect then return this node.                        */ \
    /*************************************************************************/ \
    if (AVL_TYPE_SCORE_NEAR_OR_EQUAL_IS_PERFECT(a_currentScore)) {              \
        return a_parent;                                                        \
    }                                                                           \
                                                                                \
    AVL_TYPE_SCORE_NEAR_OR_EQUAL score;                                         \
    const AVLNodeStruct(name) *result = a_parent;                               \
                                                                                \
    /*************************************************************************/ \
    /* if the left node is not null then get the best score from the left    */ \
    /* branch; if the score is better than the current score then update     */ \
    /* the current score and result.                                         */ \
    /*************************************************************************/ \
    if (a_parent->left != CLIB_NULLPTR) {                                       \
        AVL_TYPE_SCORE_NEAR_OR_EQUAL_GET_SCORE(                                 \
            (&a_parent->left->data),                                            \
            a_value,                                                            \
            (&score)                                                            \
        );                                                                      \
                                                                                \
        result = AVLNodeFunc(name, findNearOrEqual) (                           \
            a_parent->left,                                                     \
            a_value,                                                            \
            &score                                                              \
        );                                                                      \
                                                                                \
        if (AVL_TYPE_SCORE_NEAR_OR_EQUAL_IS_BETTER(a_currentScore, (&score))) { \
            result = a_parent;                                                  \
        } else {                                                                \
            AVL_TYPE_SCORE_NEAR_OR_EQUAL_COPY(                                  \
                a_currentScore,                                                 \
                (&score)                                                        \
            );                                                                  \
        }                                                                       \
    }                                                                           \
                                                                                \
    /*************************************************************************/ \
    /* if the right node is not null then get the best score from the right  */ \
    /* branch; if the score is better than the current score then update     */ \
    /* the current score and result.                                         */ \
    /*************************************************************************/ \
    if (a_parent->right != CLIB_NULLPTR) {                                      \
        AVL_TYPE_SCORE_NEAR_OR_EQUAL_GET_SCORE(                                 \
            (&a_parent->right->data),                                           \
            a_value,                                                            \
            (&score)                                                            \
        );                                                                      \
                                                                                \
        const AVLNodeStruct(name) *tmp = AVLNodeFunc(name, findNearOrEqual) (   \
            a_parent->right,                                                    \
            a_value,                                                            \
            &score                                                              \
        );                                                                      \
                                                                                \
        if (AVL_TYPE_SCORE_NEAR_OR_EQUAL_IS_BETTER((&score), a_currentScore)) { \
            AVL_TYPE_SCORE_NEAR_OR_EQUAL_COPY(                                  \
                a_currentScore,                                                 \
                (&score)                                                        \
            );                                                                  \
            result = tmp;                                                       \
        }                                                                       \
    }                                                                           \
                                                                                \
    return result;                                                              \
}

#else // AVL_USE_FIND_NEAR_OR_EQUAL is not defined

#define IMPLEMENT_AVL_NODE_FUNC_FIND_NEAR_OR_EQUAL(type, name)

#endif // end if AVL_USE_FIND_NEAR_OR_EQUAL

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_NEAR_OR_EQUAL
