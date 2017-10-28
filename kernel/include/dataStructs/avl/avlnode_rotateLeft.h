#ifndef PhiOS_DATA_STRUCTURES_AVL_NODE_ROTATE_LEFT
#define PhiOS_DATA_STRUCTURES_AVL_NODE_ROTATE_LEFT

#define IMPLEMENT_AVL_NODE_FUNC_ROTATE_LEFT(type, name)                         \
static AVLNodeStruct(name)* AVLNodeFunc(name, rotateLeft) (                     \
    AVLNodeStruct(name) *a_parent)                                              \
{                                                                               \
    AVLNodeStruct(name) *newParent = a_parent->right;                           \
                                                                                \
    a_parent->right = newParent->left;                                          \
    newParent->left = a_parent;                                                 \
                                                                                \
    AVLNodeFunc(name, calculateHeight) (a_parent);                              \
    AVLNodeFunc(name, calculateHeight) (newParent);                             \
                                                                                \
    return newParent;                                                           \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_NODE_ROTATE_LEFT
