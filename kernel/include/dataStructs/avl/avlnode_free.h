#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FREE
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FREE

#ifdef AVL_USE_AVL_NODE_FREE

// void avlFree(void *a_ptr)
#ifndef AVL_FREE_FUNC
#error "free function is mandatory"
#endif

// void avlDestroyType(type *a_ptr)
#ifndef AVL_DESTROY_TYPE_FUNC
#define AVL_DESTROY_TYPE_FUNC(data)
#endif

#define DECLARE_AVL_NODE_FUNC_FREE(type, name)                                  \
avl_error_code_t AVLNodeFunc(name, free) (                                      \
    AVLNodeStruct(name) *a_node                                                 \
);

#define IMPLEMENT_AVL_NODE_FUNC_FREE(type, name)                                \
avl_error_code_t AVLNodeFunc(name, free) (                                      \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    if (a_node == CLIB_NULLPTR) {                                               \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    AVLNodeFunc(name, free)(a_node->left);                                      \
    AVLNodeFunc(name, free)(a_node->right);                                     \
                                                                                \
    AVL_DESTROY_TYPE_FUNC((&a_node->data));                                     \
                                                                                \
    AVL_FREE_FUNC(a_node);                                                      \
                                                                                \
    return AVL_ERROR_SUCCESS;                                                   \
}

#else // AVL_USE_AVL_NODE_FREE is not defined

#define DECLARE_AVL_NODE_FUNC_FREE(type, name)
#define IMPLEMENT_AVL_NODE_FUNC_FREE(type, name)

#endif // end if AVL_USE_AVL_NODE_FREE

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FREE
