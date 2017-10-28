#ifndef PhiOS_DATA_STRUCTURES_AVL_INSERT_NODE
#define PhiOS_DATA_STRUCTURES_AVL_INSERT_NODE

#define DECLARE_AVL_FUNC_INSERT_NODE(type, name)                                \
uint32 AVLFunc(name, insertNode) (                                              \
    AVLStruct(name) *a_avl,                                                     \
    AVLNodeStruct(name) *a_node                                                 \
);

#define IMPLEMENT_AVL_FUNC_INSERT_NODE(type, name)                              \
uint32 AVLFunc(name, insertNode) (                                              \
    AVLStruct(name) *a_avl,                                                     \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_node == NULL) {                                                       \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    a_avl->root = AVLNodeFunc(name, insert) (a_avl->root, a_node);              \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_INSERT_NODE
