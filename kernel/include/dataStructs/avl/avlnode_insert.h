#ifndef PhiOS_DATA_STRUCTURES_AVL_NODE_INSERT
#define PhiOS_DATA_STRUCTURES_AVL_NODE_INSERT

// bool opLess(const type* a, const type* b)
#ifndef AVL_TYPE_OPERATOR_LESS
#error "operator< is mandatory"
#endif

#define IMPLEMENT_AVL_NODE_FUNC_INSERT(type, name)                              \
static AVLNodeStruct(name)* AVLNodeFunc(name, insert) (                         \
    AVLNodeStruct(name) *a_parent,                                              \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    if (a_parent == NULL) {                                                     \
        return a_node;                                                          \
    }                                                                           \
                                                                                \
    if (AVL_TYPE_OPERATOR_LESS((&a_node->data), (&a_parent->data))) {           \
        a_parent->left = AVLNodeFunc(name, insert) (a_parent->left, a_node);    \
    } else {                                                                    \
        a_parent->right = AVLNodeFunc(name, insert) (a_parent->right, a_node);  \
    }                                                                           \
                                                                                \
    return AVLNodeFunc(name, balance) (a_parent);                               \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_NODE_INSERT
