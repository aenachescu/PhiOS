#ifndef PhiOS_DATA_STRUCTURES_AVL_NODE_BALANCE
#define PhiOS_DATA_STRUCTURES_AVL_NODE_BALANCE

#define IMPLEMENT_AVL_NODE_FUNC_BALANCE(type, name)                             \
static AVLNodeStruct(name)* AVLNodeFunc(name, balance) (                        \
    AVLNodeStruct(name) *a_parent)                                              \
{                                                                               \
    AVLNodeFunc(name, calculateHeight) (a_parent);                              \
                                                                                \
    if (AVLNodeFunc(name, diffHeight) (a_parent->left, a_parent->right) == 2) { \
        if (AVLNodeFunc(name, isHeightGreater) (                                \
                a_parent->left->right, a_parent->left->left)) {                 \
            a_parent->left = AVLNodeFunc(name, rotateLeft) (a_parent->left);    \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, rotateRight) (a_parent);                       \
    }                                                                           \
                                                                                \
    if (AVLNodeFunc(name, diffHeight) (a_parent->right, a_parent->left) == 2) { \
        if (AVLNodeFunc(name, isHeightGreater) (                                \
                a_parent->right->left, a_parent->right->right)) {               \
            a_parent->right = AVLNodeFunc(name, rotateRight) (a_parent->right); \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, rotateLeft) (a_parent);                        \
    }                                                                           \
                                                                                \
    return a_parent;                                                            \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_NODE_BALANCE
