#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE

#define DECLARE_AVL_FUNC_REMOVE(type, name)                                     \
clib_error_code_t AVLFunc(name, remove) (                                       \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value,                                                        \
    AVLNodeStruct(name) **a_result                                              \
);

#define IMPLEMENT_AVL_FUNC_REMOVE(type, name)                                   \
clib_error_code_t AVLFunc(name, remove) (                                       \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value,                                                        \
    AVLNodeStruct(name) **a_result)                                             \
{                                                                               \
    if (a_result == CLIB_NULLPTR) {                                             \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    *a_result = CLIB_NULLPTR;                                                   \
                                                                                \
    if (a_avl == CLIB_NULLPTR) {                                                \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    if (a_value == CLIB_NULLPTR) {                                              \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    if (a_avl->root == CLIB_NULLPTR) {                                          \
        return CLIB_ERROR_NOT_FOUND;                                            \
    }                                                                           \
                                                                                \
    a_avl->root = AVLNodeFunc(name, remove) (                                   \
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

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE
