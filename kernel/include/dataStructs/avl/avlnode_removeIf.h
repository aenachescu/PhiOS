#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_IF
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_IF

#define IMPLEMENT_AVL_NODE_FUNC_REMOVE_IF(type, name)                           \
static AVLNodeStruct(name)* AVLNodeFunc(name, removeIf) (                       \
    AVLNodeStruct(name) *a_parent,                                              \
    AVL_REMOVE_IF_CBK(name) a_cbk,                                              \
    void *a_context,                                                            \
    AVLNodeStruct(name) **a_result)                                             \
{                                                                               \
    if (a_parent->left != CLIB_NULLPTR) {                                       \
        a_parent->left = AVLNodeFunc(name, removeIf) (                          \
            a_parent->left,                                                     \
            a_cbk,                                                              \
            a_context,                                                          \
            a_result                                                            \
        );                                                                      \
                                                                                \
        if (*a_result != CLIB_NULLPTR) {                                        \
            return AVLNodeFunc(name, balance) (a_parent);                       \
        }                                                                       \
    }                                                                           \
                                                                                \
    if (a_cbk(&a_parent->data, a_context) == CLIB_TRUE) {                       \
        *a_result = a_parent;                                                   \
                                                                                \
        AVLNodeStruct(name) *auxNode;                                           \
                                                                                \
        if (a_parent->right == CLIB_NULLPTR) {                                  \
            auxNode = a_parent->left;                                           \
            a_parent->left = CLIB_NULLPTR;                                      \
            return auxNode;                                                     \
        }                                                                       \
                                                                                \
        AVLNodeStruct(name) *minNode = CLIB_NULLPTR;                            \
        auxNode = AVLNodeFunc(name, removeMin) (a_parent->right, &minNode);     \
                                                                                \
        minNode->right = auxNode;                                               \
        minNode->left = a_parent->left;                                         \
                                                                                \
        a_parent->left = CLIB_NULLPTR;                                          \
        a_parent->right = CLIB_NULLPTR;                                         \
                                                                                \
        return AVLNodeFunc(name, balance) (minNode);                            \
    }                                                                           \
                                                                                \
    if (a_parent->right != CLIB_NULLPTR) {                                      \
        a_parent->right = AVLNodeFunc(name, removeIf) (                         \
            a_parent->right,                                                    \
            a_cbk,                                                              \
            a_context,                                                          \
            a_result                                                            \
        );                                                                      \
                                                                                \
        if (*a_result != CLIB_NULLPTR) {                                        \
            return AVLNodeFunc(name, balance) (a_parent);                       \
        }                                                                       \
    }                                                                           \
                                                                                \
    if (*a_result == CLIB_NULLPTR) {                                            \
        return a_parent;                                                        \
    }                                                                           \
                                                                                \
    return AVLNodeFunc(name, balance) (a_parent);                               \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE_IF
