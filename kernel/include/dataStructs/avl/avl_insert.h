#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_INSERT
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_INSERT

#ifdef AVL_USE_AVL_NODE_CREATE

#define DECLARE_AVL_FUNC_INSERT(type, name)                                     \
clib_error_code_t AVLFunc(name, insert) (                                       \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value                                                         \
);

#define IMPLEMENT_AVL_FUNC_INSERT(type, name)                                   \
clib_error_code_t AVLFunc(name, insert) (                                       \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value)                                                        \
{                                                                               \
    INPUT_CHECK(a_avl == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)                 \
    INPUT_CHECK(a_value == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER)               \
                                                                                \
    AVLNodeStruct(name) *node = CLIB_NULLPTR;                                   \
    clib_error_code_t err = AVLNodeFunc(name, create) (&node, a_value);         \
    if (err != CLIB_ERROR_SUCCESS) {                                            \
        return err;                                                             \
    }                                                                           \
                                                                                \
    a_avl->root = AVLNodeFunc(name, insert) (a_avl->root, node);                \
                                                                                \
    return CLIB_ERROR_SUCCESS;                                                  \
}

#else // AVL_USE_AVL_NODE_CREATE is not defined

#define DECLARE_AVL_FUNC_INSERT(type, name)
#define IMPLEMENT_AVL_FUNC_INSERT(type, name)

#endif // end if AVL_USE_AVL_NODE_CREATE

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_INSERT
