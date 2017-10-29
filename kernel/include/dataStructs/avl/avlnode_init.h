#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_INIT
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_INIT

// uint32 copyType(type *a_dest, const type *a_src)
#ifndef AVL_COPY_TYPE_FUNC
#error "copy function is mandatory"
#endif

#define DECLARE_AVL_NODE_FUNC_INIT(type, name)                                  \
uint32 AVLNodeFunc(name, init) (                                                \
    AVLNodeStruct(name) *a_node,                                                \
    const type *a_data                                                          \
);

#define IMPLEMENT_AVL_NODE_FUNC_INIT(type, name)                                \
uint32 AVLNodeFunc(name, init) (                                                \
    AVLNodeStruct(name) *a_node,                                                \
    const type *a_data)                                                         \
{                                                                               \
    if (a_node == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_data == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    a_node->left = NULL;                                                        \
    a_node->right = NULL;                                                       \
    a_node->height = 1;                                                         \
                                                                                \
    uint32 err = AVL_COPY_TYPE_FUNC((&a_node->data), a_data);                   \
    if (err != ERROR_SUCCESS) {                                                 \
        return err;                                                             \
    }                                                                           \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_INIT
