#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_TYPE
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_TYPE

#define DECLARE_AVL_FUNC_FIND_TYPE(type, name)                                  \
avl_error_code_t AVLFunc(name, findType) (                                      \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const type **a_res                                                          \
);

#define IMPLEMENT_AVL_FUNC_FIND_TYPE(type, name)                                \
avl_error_code_t AVLFunc(name, findType) (                                      \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const type **a_res)                                                         \
{                                                                               \
    if (a_res == NULL) {                                                        \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    *a_res = NULL;                                                              \
                                                                                \
    if (a_avl == NULL) {                                                        \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    if (a_value == NULL) {                                                      \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    const AVLNodeStruct(name) *node =                                           \
        AVLNodeFunc(name, find) (a_avl->root, a_value);                         \
                                                                                \
    if (node == NULL) {                                                         \
        return AVL_ERROR_NOT_FOUND;                                             \
    }                                                                           \
                                                                                \
    *a_res = &node->data;                                                       \
                                                                                \
    return AVL_ERROR_SUCCESS;                                                   \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND_TYPE
