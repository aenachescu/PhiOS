#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_MIN
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_MIN

#define IMPLEMENT_AVL_NODE_FUNC_REMOVE_MIN(type, name)                          \
static AVLNodeStruct(name)* AVLNodeFunc(name, removeMin) (                      \
    AVLNodeStruct(name) *a_parent,                                              \
    AVLNodeStruct(name) **a_min)                                                \
{                                                                               \
    if (a_parent->left == CLIB_NULLPTR) {                                       \
        *a_min = a_parent;                                                      \
        return a_parent->right;                                                 \
    }                                                                           \
                                                                                \
    a_parent->left = AVLNodeFunc(name, removeMin) (a_parent->left, a_min);      \
                                                                                \
    return AVLNodeFunc(name, balance) (a_parent);                               \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_MIN
