#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND

#define DECLARE_AVL_FUNC_FIND(type, name)                                       \
clib_error_code_t AVLFunc(name, find) (                                         \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const AVLNodeStruct(name) **a_res                                           \
);

#define IMPLEMENT_AVL_FUNC_FIND(type, name)                                     \
clib_error_code_t AVLFunc(name, find) (                                         \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value,                                                        \
    const AVLNodeStruct(name) **a_res)                                          \
{                                                                               \
    INPUT_CHECK(a_res == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)                 \
                                                                                \
    *a_res = CLIB_NULLPTR;                                                      \
                                                                                \
    INPUT_CHECK(a_avl == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)                 \
    INPUT_CHECK(a_value == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)               \
                                                                                \
    *a_res = AVLNodeFunc(name, find) (a_avl->root, a_value);                    \
    if (*a_res == CLIB_NULLPTR) {                                               \
        return CLIB_ERROR_NOT_FOUND;                                            \
    }                                                                           \
                                                                                \
    return CLIB_ERROR_SUCCESS;                                                  \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_FIND
