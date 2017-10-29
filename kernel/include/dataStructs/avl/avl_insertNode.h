#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_INSERT_NODE
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_INSERT_NODE

#define DECLARE_AVL_FUNC_INSERT_NODE(type, name)                                \
avl_error_code_t AVLFunc(name, insertNode) (                                    \
    AVLStruct(name) *a_avl,                                                     \
    AVLNodeStruct(name) *a_node                                                 \
);

#define IMPLEMENT_AVL_FUNC_INSERT_NODE(type, name)                              \
avl_error_code_t AVLFunc(name, insertNode) (                                    \
    AVLStruct(name) *a_avl,                                                     \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    if (a_node == NULL) {                                                       \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    a_avl->root = AVLNodeFunc(name, insert) (a_avl->root, a_node);              \
                                                                                \
    return AVL_ERROR_SUCCESS;                                                   \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_INSERT_NODE
