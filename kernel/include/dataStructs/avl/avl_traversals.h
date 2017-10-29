#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_TRAVERSALS
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_TRAVERSALS

#define AVL_FOREACH_CBK(name)                                                   \
    C_TEMPLATE_DATA_STRUCTURES_CONCAT(                                          \
        name,                                                                   \
        AVLForeachCbk_t                                                         \
    )

#define DECLARE_AVL_FOR_EACH_CALLBACK_TYPE(type, name)                          \
typedef void (*AVL_FOREACH_CBK(name)) (                                         \
    const type *a_value                                                         \
);

// declare & implement for each inorder

#define DECLARE_AVL_FUNC_FOR_EACH_INORDER(type, name)                           \
avl_error_code_t AVLFunc(name, foreachInorder) (                                \
    const AVLStruct(name) *a_avl,                                               \
    AVL_FOREACH_CBK(name) a_cbk                                                 \
);

#define IMPLEMENT_AVL_FUNC_FOR_EACH_INORDER(type, name)                         \
avl_error_code_t AVLFunc(name, foreachInorder) (                                \
    const AVLStruct(name) *a_avl,                                               \
    AVL_FOREACH_CBK(name) a_cbk)                                                \
{                                                                               \
    if (a_avl == CLIB_NULLPTR) {                                                \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    if (a_cbk == CLIB_NULLPTR) {                                                \
        return AVL_ERROR_INVALID_FUNCTION;                                      \
    }                                                                           \
                                                                                \
    AVLNodeFunc(name, foreachInorder) (a_avl->root, a_cbk);                     \
                                                                                \
    return AVL_ERROR_SUCCESS;                                                   \
}

// declare & implement ofr each preorder

#define DECLARE_AVL_FUNC_FOR_EACH_PREORDER(type, name)                          \
avl_error_code_t AVLFunc(name, foreachPreorder) (                               \
    const AVLStruct(name) *a_avl,                                               \
    AVL_FOREACH_CBK(name) a_cbk                                                 \
);

#define IMPLEMENT_AVL_FUNC_FOR_EACH_PREORDER(type, name)                        \
avl_error_code_t AVLFunc(name, foreachPreorder) (                               \
    const AVLStruct(name) *a_avl,                                               \
    AVL_FOREACH_CBK(name) a_cbk)                                                \
{                                                                               \
    if (a_avl == CLIB_NULLPTR) {                                                \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    if (a_cbk == CLIB_NULLPTR) {                                                \
        return AVL_ERROR_INVALID_FUNCTION;                                      \
    }                                                                           \
                                                                                \
    AVLNodeFunc(name, foreachPreorder) (a_avl->root, a_cbk);                    \
                                                                                \
    return AVL_ERROR_SUCCESS;                                                   \
}

// declare & implement for each postorder

#define DECLARE_AVL_FUNC_FOR_EACH_POSTORDER(type, name)                         \
avl_error_code_t AVLFunc(name, foreachPostorder) (                              \
    const AVLStruct(name) *a_avl,                                               \
    AVL_FOREACH_CBK(name) a_cbk                                                 \
);

#define IMPLEMENT_AVL_FUNC_FOR_EACH_POSTORDER(type, name)                       \
avl_error_code_t AVLFunc(name, foreachPostorder) (                              \
    const AVLStruct(name) *a_avl,                                               \
    AVL_FOREACH_CBK(name) a_cbk)                                                \
{                                                                               \
    if (a_avl == CLIB_NULLPTR) {                                                \
        return AVL_ERROR_NULL_POINTER;                                          \
    }                                                                           \
                                                                                \
    if (a_cbk == CLIB_NULLPTR) {                                                \
        return AVL_ERROR_INVALID_FUNCTION;                                      \
    }                                                                           \
                                                                                \
    AVLNodeFunc(name, foreachPostorder) (a_avl->root, a_cbk);                   \
                                                                                \
    return AVL_ERROR_SUCCESS;                                                   \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_TRAVERSALS
