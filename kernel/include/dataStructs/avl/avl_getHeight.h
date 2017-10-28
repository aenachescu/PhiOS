#ifndef PhiOS_DATA_STRUCTURES_AVL_GET_HEIGHT
#define PhiOS_DATA_STRUCTURES_AVL_GET_HEIGHT

#define DECLARE_AVL_FUNC_GET_HEIGHT(type, name)                                 \
uint32 AVLFunc(name, getHeight) (                                               \
    const AVLStruct(name) *a_avl                                                \
);

#define IMPLEMENT_AVL_FUNC_GET_HEIGHT(type, name)                               \
uint32 AVLFunc(name, getHeight) (                                               \
    const AVLStruct(name) *a_avl)                                               \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return 0;                                                               \
    }                                                                           \
                                                                                \
    return AVLNodeFunc(name, getHeight) (a_avl->root);                          \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_GET_HEIGHT
