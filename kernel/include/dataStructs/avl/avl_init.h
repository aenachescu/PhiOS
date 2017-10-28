#ifndef PhiOS_DATA_STRUCTURES_AVL_INIT
#define PhiOS_DATA_STRUCTURES_AVL_INIT

#define DECLARE_AVL_FUNC_INIT(type, name)                                       \
uint32 AVLFunc(name, init) (                                                    \
    AVLStruct(name) *a_avl                                                      \
);

#define IMPLEMENT_AVL_FUNC_INIT(type, name)                                     \
uint32 AVLFunc(name, init) (                                                    \
    AVLStruct(name) *a_avl)                                                     \
{                                                                               \
    if (a_avl == NULL) {                                                        \
        return ERROR_NULL_POINTER;                                              \
    }                                                                           \
                                                                                \
    a_avl->root = NULL;                                                         \
                                                                                \
    return ERROR_SUCCESS;                                                       \
}

#endif // end if PhiOS_DATA_STRUCTURES_AVL_INIT
