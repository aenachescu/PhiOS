#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL

/*
 * Using:
 *   - use DECLARE_AVL_TYPE(type, name) to declare the avl structs and public functions.
 *   - use IMPLEMENT_AVL_TYPE(type, name) to implement the avl functions (both public
 *         private functions).
 *
 * Typedefs:
 *    1. nameAVLForeachCbk_t
 *       Signature void func(const type *a_value);
 *       Brief: You must pass an function of this type to the functions of traversals,
 *              and this function will be called for each element from AVL.
 *
 * Macro functions:
 *    1. AVL_TYPE_OPERATOR_GREATER
 *       Signature: clib_bool_t opGreater(const type *a, const type *b)
 *       Brief: return CLIB_TRUE if a > b, otherwise CLIB_FALSE.
 *
 *    2. AVL_TYPE_OPERATOR_LESS
 *       Signature: clib_bool_t opLess(const type *a, const type *b)
 *       Brief: return CLIB_TRUE if a < b, otherwise CLIB_FALSE.
 *
 *    3. AVL_COPY_TYPE_FUNC
 *       Signature: clib_error_code_t copyType(type *a_dest, const type *a_src)
 *       Brief: return CLIB_ERROR_SUCCESS if the a_src was copied into a_dest, otherwise error.
 *
 *    4. AVL_ALLOC_FUNC
 *       Signature: void* avlAlloc(size_t a_size)
 *       Brief: return an pointer where the node will be stored, or CLIB_NULLPTR if
 *              there is no available memroy.
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
 *       Signature: clib_bool_t scoreIsInvalid(const ScoreType *a_score)
 *       Brief: return CLIB_TRUE if a_score is invalid, otherwise CLIB_FALSE.
 *
 *    8. AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_PERFECT
 *       Signature: clib_bool_t scoreIsPerfect(const ScoreType *a_score)
 *       Brief: return CLIB_TRUE if the a_score is perfect, otherwise CLIB_FALSE.
 *
 *    9. AVL_TYPE_SCORE_GREATER_OR_EQUAL_IS_BETTER
 *       Signature: clib_bool_t scoreIsBetter(const ScoreType *a, const ScoreType *b)
 *       Brief: return CLIB_TRUE if the score from a is better than the score from b, otherwise CLIB_FALSE.
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
 *    1. CLIB_NULLPTR
 *       Default it has ((void*) 0x0) value, but if you have a special nullptr then
 *       you can define CLIB_NULLPTR with your value.
 *
 *    2. CLIB_TRUE
 *
 *    3. CLIB_FALSE
 *
 *    4. AVL_TYPE_SCORE_GREATER_OR_EQUAL
 *       This is a data structure (or primitive data type) where the score between
 *       the element being searched and another element it's stored.
 *
 * Global requirements:
 *   1. you must define the following macro functions:
 *        - AVL_TYPE_OPERATOR_GREATER - is used by the AVLNode_find, AVLNode_remove functions.
 *        - AVL_TYPE_OPERATOR_LESS - is used by the AVLNode_find, AVLNode_insert, AVLNode_remove functions.
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
#include "kernel/include/dataStructs/errors.h"
#include "kernel/include/dataStructs/preprocessor.h"
#include "kernel/include/dataStructs/nullptr.h"
#include "kernel/include/dataStructs/bool.h"
#include "kernel/include/dataStructs/input_checks.h"

// include avl node headers
#include "kernel/include/dataStructs/avl/avlnode_struct.h"
#include "kernel/include/dataStructs/avl/avlnode_helpers.h"
#include "kernel/include/dataStructs/avl/avlnode_rotateLeft.h"
#include "kernel/include/dataStructs/avl/avlnode_rotateRight.h"
#include "kernel/include/dataStructs/avl/avlnode_balance.h"
#include "kernel/include/dataStructs/avl/avlnode_insert.h"
#include "kernel/include/dataStructs/avl/avlnode_find.h"
#include "kernel/include/dataStructs/avl/avlnode_init.h"
#include "kernel/include/dataStructs/avl/avlnode_uninit.h"
#include "kernel/include/dataStructs/avl/avlnode_create.h"
#include "kernel/include/dataStructs/avl/avlnode_free.h"
#include "kernel/include/dataStructs/avl/avlnode_findGreaterOrEqual.h"
#include "kernel/include/dataStructs/avl/avlnode_traversals.h"
#include "kernel/include/dataStructs/avl/avlnode_isBalanced.h"
#include "kernel/include/dataStructs/avl/avlnode_remove.h"
#include "kernel/include/dataStructs/avl/avlnode_removeMin.h"
#include "kernel/include/dataStructs/avl/avlnode_findIf.h"

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
#include "kernel/include/dataStructs/avl/avl_traversals.h"
#include "kernel/include/dataStructs/avl/avl_isBalanced.h"
#include "kernel/include/dataStructs/avl/avl_remove.h"
#include "kernel/include/dataStructs/avl/avl_findIf.h"

// declare avl structures
#define DECLARE_AVL_STRUCTS(type, name)                                         \
    DECLARE_AVL_NODE_STRUCT(type, name)                                         \
    DECLARE_AVL_STRUCT(type, name)                                              \
    DECLARE_AVL_FOR_EACH_CALLBACK_TYPE(type, name)                              \
    DECLARE_AVL_FIND_IF_CALLBACK_TYPE(type, name)

// declare avl node public functions
#define DECLARE_AVL_NODE_FUNC(type, name)                                       \
    DECLARE_AVL_NODE_FUNC_INIT(type, name)                                      \
    DECLARE_AVL_NODE_FUNC_UNINIT(type, name)                                    \
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
    DECLARE_AVL_FUNC_FIND_GREATER_OR_EQUAL(type, name)                          \
    DECLARE_AVL_FUNC_FOR_EACH_INORDER(type, name)                               \
    DECLARE_AVL_FUNC_FOR_EACH_PREORDER(type, name)                              \
    DECLARE_AVL_FUNC_FOR_EACH_POSTORDER(type, name)                             \
    DECLARE_AVL_FUNC_IS_BALANCED(type, name)                                    \
    DECLARE_AVL_FUNC_REMOVE(type, name)                                         \
    DECLARE_AVL_FUNC_FIND_IF(type, name)

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
    IMPLEMENT_AVL_NODE_FUNC_UNINIT(type, name)                                  \
    IMPLEMENT_AVL_NODE_FUNC_CREATE(type, name)                                  \
    IMPLEMENT_AVL_NODE_FUNC_FREE(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_FIND(type, name)                                    \
    IMPLEMENT_AVL_NODE_FUNC_FIND_GREATER_OR_EQUAL(type, name)                   \
    IMPLEMENT_AVL_NODE_FUNC_FOR_EACH_INORDER(type, name)                        \
    IMPLEMENT_AVL_NODE_FUNC_FOR_EACH_PREORDER(type, name)                       \
    IMPLEMENT_AVL_NODE_FUNC_FOR_EACH_POSTORDER(type, name)                      \
    IMPLEMENT_AVL_NODE_FUNC_IS_BALANCED(type, name)                             \
    IMPLEMENT_AVL_NODE_FUNC_REMOVE_MIN(type, name)                              \
    IMPLEMENT_AVL_NODE_FUNC_REMOVE(type, name)                                  \
    IMPLEMENT_AVL_NODE_FUNC_FIND_IF(type, name)

// implement avl functions
#define IMPLEMENT_AVL_FUNC(type, name)                                          \
    IMPLEMENT_AVL_FUNC_INIT(type, name)                                         \
    IMPLEMENT_AVL_FUNC_FREE(type, name)                                         \
    IMPLEMENT_AVL_FUNC_GET_HEIGHT(type, name)                                   \
    IMPLEMENT_AVL_FUNC_INSERT_NODE(type, name)                                  \
    IMPLEMENT_AVL_FUNC_INSERT(type, name)                                       \
    IMPLEMENT_AVL_FUNC_FIND(type, name)                                         \
    IMPLEMENT_AVL_FUNC_FIND_TYPE(type, name)                                    \
    IMPLEMENT_AVL_FUNC_FIND_GREATER_OR_EQUAL(type, name)                        \
    IMPLEMENT_AVL_FUNC_FOR_EACH_INORDER(type, name)                             \
    IMPLEMENT_AVL_FUNC_FOR_EACH_PREORDER(type, name)                            \
    IMPLEMENT_AVL_FUNC_FOR_EACH_POSTORDER(type, name)                           \
    IMPLEMENT_AVL_FUNC_IS_BALANCED(type, name)                                  \
    IMPLEMENT_AVL_FUNC_REMOVE(type, name)                                       \
    IMPLEMENT_AVL_FUNC_FIND_IF(type, name)

// implement avl type
#define IMPLEMENT_AVL_TYPE(type, name)                                          \
    IMPLEMENT_AVL_NODE_FUNC(type, name)                                         \
    IMPLEMENT_AVL_FUNC(type, name)

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL
