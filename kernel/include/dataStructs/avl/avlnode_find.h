#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND

// bool opGreater(const type *a, const type *b)
#ifndef AVL_TYPE_OPERATOR_GREATER
#error "operator> is mandatory "
#endif

// bool opLess(const type *a, const type *b)
#ifndef AVL_TYPE_OPERATOR_LESS
#error "operator< is mandatory"
#endif

#define IMPLEMENT_AVL_NODE_FUNC_FIND(type, name)                                \
static const AVLNodeStruct(name)* AVLNodeFunc(name, find) (                     \
    const AVLNodeStruct(name) *a_parent,                                        \
    const type *a_value)                                                        \
{                                                                               \
    if (a_parent == CLIB_NULLPTR) {                                             \
        return CLIB_NULLPTR;                                                    \
    }                                                                           \
                                                                                \
    if (AVL_TYPE_OPERATOR_LESS(a_value, (&a_parent->data))) {                   \
        return AVLNodeFunc(name, find) (a_parent->left, a_value);               \
    }                                                                           \
                                                                                \
    if (AVL_TYPE_OPERATOR_GREATER(a_value, (&a_parent->data))) {                \
        return AVLNodeFunc(name, find) (a_parent->right, a_value);              \
    }                                                                           \
                                                                                \
    return a_parent;                                                            \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_FIND
