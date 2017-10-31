#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_UNINIT
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_UNINIT

// void avlDestroyType(type *a_ptr)
#ifndef AVL_DESTROY_TYPE_FUNC
#define AVL_DESTROY_TYPE_FUNC(data)
#endif

#define DECLARE_AVL_NODE_FUNC_UNINIT(type, name)                                \
clib_error_code_t AVLNodeFunc(name, uninit) (                                   \
    AVLNodeStruct(name) *a_node                                                 \
);

#define IMPLEMENT_AVL_NODE_FUNC_UNINIT(type, name)                              \
clib_error_code_t AVLNodeFunc(name, uninit) (                                   \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    if (a_node == CLIB_NULLPTR) {                                               \
        return CLIB_ERROR_NULL_POINTER;                                         \
    }                                                                           \
                                                                                \
    a_node->left = CLIB_NULLPTR;                                                \
    a_node->right = CLIB_NULLPTR;                                               \
    a_node->height = 0;                                                         \
                                                                                \
    AVL_DESTROY_TYPE_FUNC((&a_node->data));                                     \
                                                                                \
    return CLIB_ERROR_SUCCESS;                                                  \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_UNINIT
