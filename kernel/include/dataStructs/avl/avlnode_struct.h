#ifndef PhiOS_DATA_STRUCTURES_AVL_NODE_STRUCT
#define PhiOS_DATA_STRUCTURES_AVL_NODE_STRUCT

#define AVLNodeStruct(name) CONCAT(name, AVLNode)
#define AVLNodeFunc(name, func) CONCAT(AVLNodeStruct(name), CONCAT(_, func))

#define DECLARE_AVL_NODE_STRUCT(type, name)                                     \
typedef struct CONCAT(AVLNodeStruct(name), _t)                                  \
{                                                                               \
    struct CONCAT(AVLNodeStruct(name), _t) *left;                               \
    struct CONCAT(AVLNodeStruct(name), _t) *right;                              \
    uint32 height;                                                              \
    type data;                                                                  \
} AVLNodeStruct(name);

#endif // end if PhiOS_DATA_STRUCTURES_AVL_NODE_STRUCT
