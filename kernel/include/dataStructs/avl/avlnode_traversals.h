#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_TRAVERSALS
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_TRAVERSALS

// declare & implement avl node for each inorder

#define DECLARE_AVL_NODE_FUNC_FOR_EACH_INORDER(type, name)                      \
static void AVLNodeFunc(name, foreachInorder) (                                 \
    const AVLNodeStruct(name) *a_parent,                                        \
    AVL_FOREACH_CBK(name) a_cbk                                                 \
);

#define IMPLEMENT_AVL_NODE_FUNC_FOR_EACH_INORDER(type, name)                    \
static void AVLNodeFunc(name, foreachInorder) (                                 \
    const AVLNodeStruct(name) *a_parent,                                        \
    AVL_FOREACH_CBK(name) a_cbk)                                                \
{                                                                               \
    if (a_parent == CLIB_NULLPTR) {                                             \
        return;                                                                 \
    }                                                                           \
                                                                                \
    AVLNodeFunc(name, foreachInorder) (a_parent->left, a_cbk);                  \
    a_cbk((&a_parent->data));                                                   \
    AVLNodeFunc(name, foreachInorder) (a_parent->right, a_cbk);                 \
}

// declare & implement avl node for each preorder

#define DECLARE_AVL_NODE_FUNC_FOR_EACH_PREORDER(type, name)                     \
static void AVLNodeFunc(name, foreachPreorder) (                                \
    const AVLNodeStruct(name) *a_parent,                                        \
    AVL_FOREACH_CBK(name) a_cbk                                                 \
);

#define IMPLEMENT_AVL_NODE_FUNC_FOR_EACH_PREORDER(type, name)                   \
static void AVLNodeFunc(name, foreachPreorder) (                                \
    const AVLNodeStruct(name) *a_parent,                                        \
    AVL_FOREACH_CBK(name) a_cbk)                                                \
{                                                                               \
    if (a_parent == CLIB_NULLPTR) {                                             \
        return;                                                                 \
    }                                                                           \
                                                                                \
    a_cbk((&a_parent->data));                                                   \
    AVLNodeFunc(name, foreachPreorder) (a_parent->left, a_cbk);                 \
    AVLNodeFunc(name, foreachPreorder) (a_parent->right, a_cbk);                \
}

// delcare & implement avl node for each postorder

#define DECLARE_AVL_NODE_FUNC_FOR_EACH_POSTORDER(type, name)                    \
static void AVLNodeFunc(name, foreachPostorder) (                               \
    const AVLNodeStruct(name) *a_parent,                                        \
    AVL_FOREACH_CBK(name) a_cbk                                                 \
);

#define IMPLEMENT_AVL_NODE_FUNC_FOR_EACH_POSTORDER(type, name)                  \
static void AVLNodeFunc(name, foreachPostorder) (                               \
    const AVLNodeStruct(name) *a_parent,                                        \
    AVL_FOREACH_CBK(name) a_cbk)                                                \
{                                                                               \
    if (a_parent == CLIB_NULLPTR) {                                             \
        return;                                                                 \
    }                                                                           \
                                                                                \
    AVLNodeFunc(name, foreachPostorder) (a_parent->left, a_cbk);                \
    AVLNodeFunc(name, foreachPostorder) (a_parent->right, a_cbk);               \
    a_cbk((&a_parent->data));                                                   \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_TRAVERSALS
