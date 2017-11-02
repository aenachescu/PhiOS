#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_IS_BALANCED
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_IS_BALANCED

#define IMPLEMENT_AVL_NODE_FUNC_IS_BALANCED(type, name)                         \
static clib_bool_t AVLNodeFunc(name, isBalanced) (                              \
    const AVLNodeStruct(name) *a_parent)                                        \
{                                                                               \
    if (a_parent == CLIB_NULLPTR) {                                             \
        return CLIB_TRUE;                                                       \
    }                                                                           \
                                                                                \
    if (AVLNodeFunc(name, diffHeight) (a_parent->left, a_parent->right) > 1) {  \
        return CLIB_FALSE;                                                      \
    }                                                                           \
                                                                                \
    if (AVLNodeFunc(name, diffHeight) (a_parent->right, a_parent->left) > 1) {  \
        return CLIB_FALSE;                                                      \
    }                                                                           \
                                                                                \
    if (AVLNodeFunc(name, isBalanced) (a_parent->left) == CLIB_FALSE) {         \
        return CLIB_FALSE;                                                      \
    }                                                                           \
                                                                                \
    if (AVLNodeFunc(name, isBalanced) (a_parent->right) == CLIB_FALSE) {        \
        return CLIB_FALSE;                                                      \
    }                                                                           \
                                                                                \
    return CLIB_TRUE;                                                           \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_IS_BALANCED
