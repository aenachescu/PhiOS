#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_HELPERS
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_HELPERS

#define IMPLEMENT_AVL_NODE_HELPERS(type, name)                                  \
    IMPLEMENT_AVL_NODE_HELPER_GET_HEIGHT(type, name)                            \
    IMPLEMENT_AVL_NODE_HELPER_GET_MAX_HEIGHT(type, name)                        \
    IMPLEMENT_AVL_NODE_HELPER_CALCULATE_HEIGHT(type, name)                      \
    IMPLEMENT_AVL_NODE_HELPER_IS_HEIGHT_GREATER(type, name)                     \
    IMPLEMENT_AVL_NODE_HELPER_DIFF_HEIGHT(type, name)

#define IMPLEMENT_AVL_NODE_HELPER_GET_HEIGHT(type, name)                        \
static inline unsigned int AVLNodeFunc(name, getHeight) (                       \
    const AVLNodeStruct(name) *a_node)                                          \
{                                                                               \
    return a_node ? a_node->height : 0;                                         \
}

#define IMPLEMENT_AVL_NODE_HELPER_GET_MAX_HEIGHT(type, name)                    \
static inline unsigned int AVLNodeFunc(name, getMaxHeight) (                    \
    const AVLNodeStruct(name) *a_node1,                                         \
    const AVLNodeStruct(name) *a_node2)                                         \
{                                                                               \
    unsigned int height1 = AVLNodeFunc(name, getHeight) (a_node1);              \
    unsigned int height2 = AVLNodeFunc(name, getHeight) (a_node2);              \
                                                                                \
    return height1 > height2 ? height1 : height2;                               \
}

#define IMPLEMENT_AVL_NODE_HELPER_CALCULATE_HEIGHT(type, name)                  \
static inline void AVLNodeFunc(name, calculateHeight) (                         \
    AVLNodeStruct(name) *a_node)                                                \
{                                                                               \
    a_node->height =                                                            \
        1 + AVLNodeFunc(name, getMaxHeight) (a_node->left, a_node->right);      \
}

#define IMPLEMENT_AVL_NODE_HELPER_IS_HEIGHT_GREATER(type, name)                 \
static inline clib_bool_t AVLNodeFunc(name, isHeightGreater) (                  \
    const AVLNodeStruct(name) *a_node1,                                         \
    const AVLNodeStruct(name) *a_node2)                                         \
{                                                                               \
    if (AVLNodeFunc(name, getHeight) (a_node1) >                                \
        AVLNodeFunc(name, getHeight) (a_node2)) {                               \
        return CLIB_TRUE;                                                       \
    }                                                                           \
                                                                                \
    return CLIB_FALSE;                                                          \
}

#define IMPLEMENT_AVL_NODE_HELPER_DIFF_HEIGHT(type, name)                       \
static inline unsigned int AVLNodeFunc(name, diffHeight) (                      \
    AVLNodeStruct(name) *a_node1,                                               \
    AVLNodeStruct(name) *a_node2)                                               \
{                                                                               \
    unsigned int height1 = AVLNodeFunc(name, getHeight) (a_node1);              \
    unsigned int height2 = AVLNodeFunc(name, getHeight) (a_node2);              \
                                                                                \
    if (height1 > height2) {                                                    \
        return height1 - height2;                                               \
    }                                                                           \
                                                                                \
    return 0;                                                                   \
}

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_NODE_HELPERS
