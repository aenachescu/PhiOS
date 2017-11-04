#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_GET_HEIGHT
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_GET_HEIGHT

#define DECLARE_AVL_FUNC_GET_HEIGHT(type, name)                                 \
clib_error_code_t AVLFunc(name, getHeight) (                                    \
    const AVLStruct(name) *a_avl,                                               \
    unsigned int *a_res                                                         \
);

#define IMPLEMENT_AVL_FUNC_GET_HEIGHT(type, name)                               \
clib_error_code_t AVLFunc(name, getHeight) (                                    \
    const AVLStruct(name) *a_avl,                                               \
    unsigned int *a_res)                                                        \
{                                                                               \
    INPUT_CHECK(a_avl == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)                 \
    INPUT_CHECK(a_res == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)                 \
                                                                                \
    *a_res = AVLNodeFunc(name, getHeight) (a_avl->root);                        \
                                                                                \
    return CLIB_ERROR_SUCCESS;                                                  \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_GET_HEIGHT
