#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_STRUCT
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_STRUCT

#define AVLStruct(name)                                                         \
    C_TEMPLATE_DATA_STRUCTURES_CONCAT(                                          \
        name,                                                                   \
        AVL                                                                     \
    )

#define AVLStruct_t(name)                                                       \
    C_TEMPLATE_DATA_STRUCTURES_CONCAT(                                          \
        AVLStruct(name),                                                        \
        _t                                                                      \
    )

#define AVLFunc(name, func)                                                     \
    C_TEMPLATE_DATA_STRUCTURES_CONCAT(                                          \
        AVLStruct(name),                                                        \
        C_TEMPLATE_DATA_STRUCTURES_CONCAT(                                      \
            _,                                                                  \
            func                                                                \
        )                                                                       \
    )

#define DECLARE_AVL_STRUCT(type, name)                                          \
typedef struct AVLStruct_t(name)                                                \
{                                                                               \
    AVLNodeStruct(name) *root;                                                  \
} AVLStruct(name);

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_STRUCT
