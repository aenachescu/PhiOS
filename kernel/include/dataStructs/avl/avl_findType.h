#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_TYPE
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_TYPE

#define DECLARE_AVL_FUNC_FIND_TYPE(type, name)                                  \
clib_error_code_t AVLFunc(name, findType) (                                     \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const type **a_res                                                          \
);

#define IMPLEMENT_AVL_FUNC_FIND_TYPE(type, name)                                \
clib_error_code_t AVLFunc(name, findType) (                                     \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const type **a_res)                                                         \
{                                                                               \
    if (a_res == CLIB_NULLPTR) {                                                \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    *a_res = CLIB_NULLPTR;                                                      \
                                                                                \
    if (a_avl == CLIB_NULLPTR) {                                                \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    if (a_value == CLIB_NULLPTR) {                                              \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    const AVLNodeStruct(name) *node =                                           \
        AVLNodeFunc(name, find) (a_avl->root, a_value);                         \
                                                                                \
    if (node == CLIB_NULLPTR) {                                                 \
        return CLIB_ERROR_NOT_FOUND;                                            \
    }                                                                           \
                                                                                \
    *a_res = &node->data;                                                       \
                                                                                \
    return CLIB_ERROR_SUCCESS;                                                  \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_TYPE
