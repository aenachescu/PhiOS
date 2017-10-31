#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_INIT
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_INIT

// clib_error_code_t copyType(type *a_dest, const type *a_src)
#ifndef AVL_COPY_TYPE_FUNC
#error "copy function is mandatory"
#endif

#define DECLARE_AVL_NODE_FUNC_INIT(type, name)                                  \
clib_error_code_t AVLNodeFunc(name, init) (                                     \
    AVLNodeStruct(name) *a_node,                                                \
    const type *a_data                                                          \
);

#define IMPLEMENT_AVL_NODE_FUNC_INIT(type, name)                                \
clib_error_code_t AVLNodeFunc(name, init) (                                     \
    AVLNodeStruct(name) *a_node,                                                \
    const type *a_data)                                                         \
{                                                                               \
    if (a_node == CLIB_NULLPTR) {                                               \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    if (a_data == CLIB_NULLPTR) {                                               \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    a_node->left = CLIB_NULLPTR;                                                \
    a_node->right = CLIB_NULLPTR;                                               \
    a_node->height = 1;                                                         \
                                                                                \
    clib_error_code_t err = AVL_COPY_TYPE_FUNC((&a_node->data), a_data);        \
    if (err != CLIB_ERROR_SUCCESS) {                                            \
        return err;                                                             \
    }                                                                           \
                                                                                \
    return CLIB_ERROR_SUCCESS;                                                  \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_INIT
