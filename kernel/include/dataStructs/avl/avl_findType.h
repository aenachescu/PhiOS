#ifndef PhiOS_DATA_STRUCTURES_AVL_FIND_TYPE
#define PhiOS_DATA_STRUCTURES_AVL_FIND_TYPE

#define DECLARE_AVL_FUNC_FIND_TYPE(type, name)                                  \
const type* AVLFunc(name, findType) (                                           \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value                                                         \
);

#define IMPLEMENT_AVL_FUNC_FIND_TYPE(type, name)                                \
const type* AVLFunc(name, findType) (                                           \
    const AVLStruct(name) *a_avl,                                               \
    const type *a_value)                                                        \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    if (a_value == NULL) {                                                      \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    const AVLNodeStruct(name) *node =                                           \
        AVLNodeFunc(name, find) (a_avl->root, a_value);                         \
                                                                                \
    if (node == NULL) {                                                         \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    return &node->data;                                                         \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_FIND_TYPE
