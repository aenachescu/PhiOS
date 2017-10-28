#ifndef PhiOS_DATA_STRUCTURES_AVL_NODE_ROTATE_RIGHT
#define PhiOS_DATA_STRUCTURES_AVL_NODE_ROTATE_RIGHT

#define IMPLEMENT_AVL_NODE_FUNC_ROTATE_RIGHT(type, name)                        \
static AVLNodeStruct(name)* AVLNodeFunc(name, rotateRight) (                    \
    AVLNodeStruct(name) *a_parent)                                              \
{                                                                               \
    AVLNodeStruct(name) *newParent = a_parent->left;                            \
                                                                                \
    a_parent->left = newParent->right;                                          \
    newParent->right = a_parent;                                                \
                                                                                \
    AVLNodeFunc(name, calculateHeight) (a_parent);                              \
    AVLNodeFunc(name, calculateHeight) (newParent);                             \
                                                                                \
    return newParent;                                                           \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_NODE_ROTATE_RIGHT
