#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE

// clib_bool_t opGreater(const type *a, const type *b)
#ifndef AVL_TYPE_OPERATOR_GREATER
#error "operator> is mandatory "
#endif

// clib_bool_t opLess(const type *a, const type *b)
#ifndef AVL_TYPE_OPERATOR_LESS
#error "operator< is mandatory"
#endif

#define IMPLEMENT_AVL_NODE_FUNC_REMOVE(type, name)                              \
static AVLNodeStruct(name)* AVLNodeFunc(name, remove) (                         \
    AVLNodeStruct(name) *a_parent,                                              \
    const type *a_value,                                                        \
    AVLNodeStruct(name) **a_result)                                             \
{                                                                               \
    if (AVL_TYPE_OPERATOR_LESS(a_value, (&a_parent->data))) {                   \
        if (a_parent->left == CLIB_NULLPTR) {                                   \
            return a_parent;                                                    \
        }                                                                       \
                                                                                \
        a_parent->left = AVLNodeFunc(name, remove) (                            \
            a_parent->left,                                                     \
            a_value,                                                            \
            a_result                                                            \
        );                                                                      \
                                                                                \
        if (*a_result == CLIB_NULLPTR) {                                        \
            return a_parent;                                                    \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, balance) (a_parent);                           \
    }                                                                           \
                                                                                \
    if (AVL_TYPE_OPERATOR_GREATER(a_value, (&a_parent->data))) {                \
        if (a_parent->right == CLIB_NULLPTR) {                                  \
            return a_parent;                                                    \
        }                                                                       \
                                                                                \
        a_parent->right = AVLNodeFunc(name, remove) (                           \
            a_parent->right,                                                    \
            a_value,                                                            \
            a_result                                                            \
        );                                                                      \
                                                                                \
        if (*a_result == CLIB_NULLPTR) {                                        \
            return a_parent;                                                    \
        }                                                                       \
                                                                                \
        return AVLNodeFunc(name, balance) (a_parent);                           \
    }                                                                           \
                                                                                \
    *a_result = a_parent;                                                       \
                                                                                \
    AVLNodeStruct(name) *auxNode;                                               \
                                                                                \
    if (a_parent->right == CLIB_NULLPTR) {                                      \
        auxNode = a_parent->left;                                               \
        a_parent->left = CLIB_NULLPTR;                                          \
        return auxNode;                                                         \
    }                                                                           \
                                                                                \
    AVLNodeStruct(name) *minNode = CLIB_NULLPTR;                                \
    auxNode = AVLNodeFunc(name, removeMin) (a_parent->right, &minNode);         \
                                                                                \
    minNode->right = auxNode;                                                   \
    minNode->left = a_parent->left;                                             \
                                                                                \
    a_parent->left = CLIB_NULLPTR;                                              \
    a_parent->right = CLIB_NULLPTR;                                             \
                                                                                \
    return AVLNodeFunc(name, balance) (minNode);                                \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_REMOVE
