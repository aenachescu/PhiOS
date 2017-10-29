#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND

#define DECLARE_AVL_FUNC_FIND(type, name)                                       \
avl_error_code_t AVLFunc(name, find) (                                          \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const AVLNodeStruct(name) **a_res                                           \
);

#define IMPLEMENT_AVL_FUNC_FIND(type, name)                                     \
avl_error_code_t AVLFunc(name, find) (                                          \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const AVLNodeStruct(name) **a_res)                                          \
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
    *a_res = AVLNodeFunc(name, find) (a_avl->root, a_value);                    \
    if (*a_res == NULL) {                                                       \
        return AVL_ERROR_NOT_FOUND;                                             \
    }                                                                           \
                                                                                \
    return AVL_ERROR_SUCCESS;                                                   \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND
