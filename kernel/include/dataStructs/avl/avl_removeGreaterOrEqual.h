#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_GREATER_OR_EQUAL
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_GREATER_OR_EQUAL

#ifdef AVL_USE_REMOVE_GREATER_OR_EQUAL

#define DECLARE_AVL_FUNC_REMOVE_GREATER_OR_EQUAL(type, name)                    \
clib_error_code_t AVLFunc(name, removeGreaterOrEqual) (                         \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value,                                                        \
    AVLNodeStruct(name) **a_result                                              \
);

#define IMPLEMENT_AVL_FUNC_REMOVE_GREATER_OR_EQUAL(type, name)                  \
clib_error_code_t AVLFunc(name, removeGreaterOrEqual) (                         \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value,                                                        \
    AVLNodeStruct(name) **a_result)                                             \
{                                                                               \
    INPUT_CHECK(a_result == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER);             \
    *a_result = CLIB_NULLPTR;                                                   \
    INPUT_CHECK(a_avl == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER);                \
    INPUT_CHECK(a_value == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER);              \
                                                                                \
    if (a_avl->root == CLIB_NULLPTR) {                                          \
        return CLIB_ERROR_NOT_FOUND;                                            \
    }                                                                           \
                                                                                \
    a_avl->root = AVLNodeFunc(name, removeGreaterOrEqual) (                     \
        a_avl->root,                                                            \
        a_value,                                                                \
        a_result                                                                \
    );                                                                          \
                                                                                \
    if (*a_result == CLIB_NULLPTR) {                                            \
        return CLIB_ERROR_NOT_FOUND;                                            \
    }                                                                           \
                                                                                \
    return CLIB_ERROR_SUCCESS;                                                  \
}

#else // AVL_USE_REMOVE_GREATER_OR_EQUAL is not defined

#define DECLARE_AVL_FUNC_REMOVE_GREATER_OR_EQUAL(type, name)
#define IMPLEMENT_AVL_FUNC_REMOVE_GREATER_OR_EQUAL(type, name)

#endif // end if AVL_USE_REMOVE_GREATER_OR_EQUAL

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_GREATER_OR_EQUAL
