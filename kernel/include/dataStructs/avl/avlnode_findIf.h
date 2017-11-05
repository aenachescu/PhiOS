#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_IF
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_IF

#define IMPLEMENT_AVL_NODE_FUNC_FIND_IF(type, name)                             \
static const AVLNodeStruct(name)* AVLNodeFunc(name, findIf) (                   \
    const AVLNodeStruct(name) *a_parent,                                        \
    AVL_FIND_IF_CBK(name) a_cbk,                                                \
    void *a_context)                                                            \
{                                                                               \
    const AVLNodeStruct(name) *result;                                          \
    if (a_parent->left != CLIB_NULLPTR) {                                       \
        result = AVLNodeFunc(name, findIf) (a_parent->left, a_cbk, a_context);  \
        if (result != CLIB_NULLPTR) {                                           \
            return result;                                                      \
        }                                                                       \
    }                                                                           \
                                                                                \
    if (a_cbk(&a_parent->data, a_context) == CLIB_TRUE) {                       \
        return a_parent;                                                        \
    }                                                                           \
                                                                                \
    if (a_parent->right != CLIB_NULLPTR) {                                      \
        result = AVLNodeFunc(name, findIf) (a_parent->right, a_cbk, a_context); \
        if (result != CLIB_NULLPTR) {                                           \
            return result;                                                      \
        }                                                                       \
    }                                                                           \
                                                                                \
    return CLIB_NULLPTR;                                                        \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND_IF
