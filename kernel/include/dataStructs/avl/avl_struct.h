#ifndef PhiOS_DATA_STRUCTURES_AVL_STRUCT
#define PhiOS_DATA_STRUCTURES_AVL_STRUCT

#define AVLStruct(name) CONCAT(name, AVL)
#define AVLFunc(name, func) CONCAT(AVLStruct(name), CONCAT(_, func))

#define DECLARE_AVL_STRUCT(type, name)                                          \
typedef struct CONCAT(AVLStruct(name), _t)                                      \
{                                                                               \
    AVLNodeStruct(name) *root;                                                  \
} AVLStruct(name);

#endif // end if PhiOS_DATA_STRUCTURES_AVL_STRUCT
