#ifndef PhiOS_DATA_STRUCTURES_AVL_GET_HEIGHT
#define PhiOS_DATA_STRUCTURES_AVL_GET_HEIGHT

#define DECLARE_AVL_FUNC_GET_HEIGHT(type, name)                                 \
uint32 AVLFunc(name, getHeight) (                                               \
    const AVLStruct(name) *a_avl,                                               \
    uint32 *a_res                                                               \
);

#define IMPLEMENT_AVL_FUNC_GET_HEIGHT(type, name)                               \
uint32 AVLFunc(name, getHeight) (                                               \
    const AVLStruct(name) *a_avl,                                               \
    uint32 *a_res)                                                              \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    if (a_res == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    *a_res = AVLNodeFunc(name, getHeight) (a_avl->root);                        \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_GET_HEIGHT
