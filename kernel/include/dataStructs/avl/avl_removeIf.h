#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_IF
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_IF

#define AVL_REMOVE_IF_CBK(name)                                                 \
    C_TEMPLATE_DATA_STRUCTURES_CONCAT(                                          \
        name,                                                                   \
        AVLRemoveIfCbk_t                                                        \
    )

#define DECLARE_AVL_REMOVE_IF_CALLBACK_TYPE(type, name)                         \
typedef clib_bool_t (*AVL_REMOVE_IF_CBK(name)) (                                \
    const type *a_value,                                                        \
    void *a_context                                                             \
);

#define DECLARE_AVL_FUNC_REMOVE_IF(type, name)                                  \
clib_error_code_t AVLFunc(name, removeIf) (                                     \
    AVLStruct(name) *a_avl,                                                     \
    AVL_REMOVE_IF_CBK(name) a_cbk,                                              \
    void *a_context,                                                            \
    AVLNodeStruct(name) **a_result                                              \
);

#define IMPLEMENT_AVL_FUNC_REMOVE_IF(type, name)                                \
clib_error_code_t AVLFunc(name, removeIf) (                                     \
    AVLStruct(name) *a_avl,                                                     \
    AVL_REMOVE_IF_CBK(name) a_cbk,                                              \
    void *a_context,                                                            \
    AVLNodeStruct(name) **a_result)                                             \
{                                                                               \
    INPUT_CHECK(a_result == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)              \
                                                                                \
    *a_result = CLIB_NULLPTR;                                                   \
                                                                                \
    INPUT_CHECK(a_avl == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)                 \
    INPUT_CHECK(a_cbk == CLIB_NULLPTR, CLIB_ERROR_INVALID_FUNCTION)             \
                                                                                \
    if (a_avl->root == CLIB_NULLPTR) {                                          \
        return CLIB_ERROR_NOT_FOUND;                                            \
    }                                                                           \
                                                                                \
    a_avl->root = AVLNodeFunc(name, removeIf) (                                 \
        a_avl->root,                                                            \
        a_cbk,                                                                  \
        a_context,                                                              \
        a_result                                                                \
    );                                                                          \
    if (*a_result != CLIB_NULLPTR) {                                            \
        return CLIB_ERROR_SUCCESS;                                              \
    }                                                                           \
                                                                                \
    return CLIB_ERROR_NOT_FOUND;                                                \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_IF
