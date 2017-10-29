#ifndef PhiOS_DATA_STRUCTURES_AVL
#define PhiOS_DATA_STRUCTURES_AVL

/*
 * Using:
 *   - use DECLARE_AVL_TYPE(type, name) to declare the avl structs and public functions.
 *   - use IMPLEMENT_AVL_TYPE(type, name) to implement the avl functions (both public
 *         private functions).
 *
 * Macro functions:
 *    1. AVL_TYPE_OPERATOR_GREATER
 *       Signature: bool opGreater(const type *a, const type *b)
 *       Brief: return true if a > b, otherwise false.
 *
 *    2. AVL_TYPE_OPERATOR_LESS
 *       Signature: bool opLess(const type *a, const type *b)
 *       Brief: return true if a < b, otherwise false.
 *
 *    3. AVL_COPY_TYPE_FUNC
 *       Signature: uint32 copyType(type *a_dest, const type *a_src)
 *       Brief: return true if the a_src was copied into a_dest, otherwise error.
 *
 *    4. AVL_ALLOC_FUNC
 *       Signature: void* avlAlloc(size_t a_size)
 *       Brief: return an pointer where the node will be stored.
 *
 *    5. AVL_FREE_FUNC
 *       Signature: void avlFree(void *a_ptr)
 *       Brief: free the memory area where the node was stored.
 *
 *    6. AVL_DESTROY_TYPE_FUNC
 *       Signature: void avlDestroyType(type *a_ptr)
 *       Brief: destroy the object (NOT free the memory area).
 *
 *    7. AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_INVALID
 *       Signature: bool scoreIsInvalid(const ScoreType *a_score)
 *       Brief: return true if a_score is invalid, otherwise false.
 *
 *    8. AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_PERFECT
 *       Signature: bool scoreIsPerfect(const ScoreType *a_score)
 *       Brief: return true if the a_score is perfect, otherwise false.
 *
 *    9. AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_BETTER
 *       Signature: bool scoreIsBetter(const ScoreType *a, const ScoreType *b)
 *       Brief: return true if the score from a is better than the score from b, otherwise false.
 *
 *   10. AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE
 *       Signature: void getScore(const Type *a_current, const Type *a_value, ScoreType *a_res)
 *       Brief: Computes the score between a_current and a_value and stores it in a_res.
 *
 *   11. AVL_TYPE_SCORE_GREATER_OR_EQUAL_COPY
 *       Signature: void scoreCopy(ScoreType *a_dst, const ScoreType *a_src)
 *       Brief: Copies ScoreType pointed by a_src into ScoreType pointed by a_dst.
 *
 * Other macros:
 *    1. AVL_TYPE_SCORE_GREATER_OR_EQUAL
 *       This is a data structure (or primitive data type) where the score between
 *       the element being searched and another element it's stored.
 *
 * Global requirements:
 *   1. you must define the following macro functions:
 *        - AVL_TYPE_OPERATOR_GREATER - is used by the AVLNode_find function.
 *        - AVL_TYPE_OPERATOR_LESS - is used by the AVLNode_find, AVLNode_insert functions.
 *        - AVL_COPY_TYPE_FUNC - is used by the AVLNode_init, AVLNode_create functions.
 *
 * Optional functions:
 *   1. AVLNode_create() - enable this function defining AVL_USE_AVL_NODE_CREATE.
 *          Additional requirements:
 *              - AVL_ALLOC_FUNC
 *              - AVL_FREE_FUNC
 *
 *   2. AVL_insert() - enable this function defining AVL_USE_AVL_NODE_CREATE.
 *          Additional requirements: just like the AVLNode_create().
 *
 *   3. AVLNode_free() - enable this function defining AVL_USE_AVL_NODE_FREE.
 *          Additional requirements:
 *              - AVL_FREE_FUNC
 *
 *          Optional:
 *              - AVL_DESTROY_TYPE_FUNC
 *
 *   4. AVL_free() - enable this functions defining AVL_USE_AVL_NODE_FREE.
 *          Additional requirements: just line the AVLNode_free().
 *
 *   5. AVL_findGreaterOrEqual() - enable this functions defining AVL_USE_FIND_GREATER_OR_EQUAL
 *          Additional requirements:
 *              - AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_PERFECT
 *              - AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_INVALID
 *              - AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_BETTER
 *              - AVL_TYPE_SCORE_GREATER_OR_EQUAL_GET_SCORE
 *              - AVL_TYPE_SCORE_GREATER_OR_EQUAL_COPY
 *              - AVL_TYPE_SCORE_GREATER_OR_EQUAL
 */

// include basic headers
#include "kernel/include/dataStructs/avl/avl_errors.h"
#include "kernel/include/dataStructs/preprocessor.h"

// include avl node headers
#include "kernel/include/dataStructs/avl/avlnode_struct.h"
#include "kernel/include/dataStructs/avl/avlnode_helpers.h"
#include "kernel/include/dataStructs/avl/avlnode_rotateLeft.h"
#include "kernel/include/dataStructs/avl/avlnode_rotateRight.h"
#include "kernel/include/dataStructs/avl/avlnode_balance.h"
#include "kernel/include/dataStructs/avl/avlnode_insert.h"
#include "kernel/include/dataStructs/avl/avlnode_find.h"
#include "kernel/include/dataStructs/avl/avlnode_init.h"
#include "kernel/include/dataStructs/avl/avlnode_create.h"
#include "kernel/include/dataStructs/avl/avlnode_free.h"
#include "kernel/include/dataStructs/avl/avlnode_findGreaterOrEqual.h"

// include avl headers
#include "kernel/include/dataStructs/avl/avl_struct.h"
#include "kernel/include/dataStructs/avl/avl_init.h"
#include "kernel/include/dataStructs/avl/avl_free.h"
#include "kernel/include/dataStructs/avl/avl_find.h"
#include "kernel/include/dataStructs/avl/avl_findType.h"
#include "kernel/include/dataStructs/avl/avl_getHeight.h"
#include "kernel/include/dataStructs/avl/avl_insert.h"
#include "kernel/include/dataStructs/avl/avl_insertNode.h"
#include "kernel/include/dataStructs/avl/avl_findGreaterOrEqual.h"

// declare avl structures
#define DECLARE_AVL_STRUCTS(type, name)                                         \
    DECLARE_AVL_NODE_STRUCT(type, name)                                         \
    DECLARE_AVL_STRUCT(type, name)

// declare avl node public functions
#define DECLARE_AVL_NODE_FUNC(type, name)                                       \
    DECLARE_AVL_NODE_FUNC_INIT(type, name)                                      \
    DECLARE_AVL_NODE_FUNC_CREATE(type, name)                                    \
    DECLARE_AVL_NODE_FUNC_FREE(type, name)

// declare avl public functions
#define DECLARE_AVL_FUNC(type, name)                                            \
    DECLARE_AVL_FUNC_INIT(type, name)                                           \
    DECLARE_AVL_FUNC_FREE(type, name)                                           \
    DECLARE_AVL_FUNC_GET_HEIGHT(type, name)                                     \
    DECLARE_AVL_FUNC_INSERT_NODE(type, name)                                    \
    DECLARE_AVL_FUNC_INSERT(type, name)                                         \
    DECLARE_AVL_FUNC_FIND(type, name)                                           \
    DECLARE_AVL_FUNC_FIND_TYPE(type, name)                                      \
    DECLARE_AVL_FUNC_FIND_GREATER_OR_EQUAL(type, name)

// declare avl type
#define DECLARE_AVL_TYPE(type, name)                                            \
    DECLARE_AVL_STRUCTS(type, name)                                             \
    DECLARE_AVL_NODE_FUNC(type, name)                                           \
    DECLARE_AVL_FUNC(type, name)

// implement avl node functions
#define IMPLEMENT_AVL_NODE_FUNC(type, name)                                     \
    IMPLEMENT_AVL_NODE_HELPERS(type, name)                                      \
    IMPLEMENT_AVL_NODE_FUNC_ROTATE_LEFT(type, name)                             \
    IMPLEMENT_AVL_NODE_FUNC_ROTATE_RIGHT(type, name)                            \
    IMPLEMENT_AVL_NODE_FUNC_BALANCE(type, name)                                 \
    IMPLEMENT_AVL_NODE_FUNC_INSERT(type, name)                                  \
    IMPLEMENT_AVL_NODE_FUNC_INIT(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_CREATE(type, name)                                  \
    IMPLEMENT_AVL_NODE_FUNC_FREE(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_FIND(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_FIND_GREATER_OR_EQUAL(type, name)

// implement avl functions
#define IMPLEMENT_AVL_FUNC(type, name)                                          \
    IMPLEMENT_AVL_FUNC_INIT(type, name)                                         \
    IMPLEMENT_AVL_FUNC_FREE(type, name)                                         \
    IMPLEMENT_AVL_FUNC_GET_HEIGHT(type, name)                                   \
    IMPLEMENT_AVL_FUNC_INSERT_NODE(type, name)                                  \
    IMPLEMENT_AVL_FUNC_INSERT(type, name)                                       \
    IMPLEMENT_AVL_FUNC_FIND(type, name)                                         \
    IMPLEMENT_AVL_FUNC_FIND_TYPE(type, name)                                    \
    IMPLEMENT_AVL_FUNC_FIND_GREATER_OR_EQUAL(type, name)

// implement avl type
#define IMPLEMENT_AVL_TYPE(type, name)                                          \
    IMPLEMENT_AVL_NODE_FUNC(type, name)                                         \
    IMPLEMENT_AVL_FUNC(type, name)

#endif // end if PhiOS_DATA_STRUCTURES_AVL
