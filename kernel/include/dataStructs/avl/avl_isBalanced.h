#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_IS_BALANCED
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_IS_BALANCED

#define DECLARE_AVL_FUNC_IS_BALANCED(type, name)                                \
clib_error_code_t AVLFunc(name, isBalanced) (                                   \
    const AVLStruct(name) *a_avl,                                               \
    clib_bool_t *a_result                                                       \
);

#define IMPLEMENT_AVL_FUNC_IS_BALANCED(type, name)                              \
clib_error_code_t AVLFunc(name, isBalanced) (                                   \
    const AVLStruct(name) *a_avl,                                               \
    clib_bool_t *a_result)                                                      \
{                                                                               \
    INPUT_CHECK(a_avl == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)                 \
    INPUT_CHECK(a_result == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)              \
                                                                                \
    *a_result = AVLNodeFunc(name, isBalanced) (a_avl->root);                    \
                                                                                \
    return CLIB_ERROR_SUCCESS;                                                  \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_IS_BALANCED
