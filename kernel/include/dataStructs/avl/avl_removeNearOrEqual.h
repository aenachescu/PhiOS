#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_NEAR_OR_EQUAL
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_NEAR_OR_EQUAL

#ifdef AVL_USE_REMOVE_NEAR_OR_EQUAL

#define AVL_USE_FIND_NEAR_OR_EQUAL

#define DECLARE_AVL_FUNC_REMOVE_NEAR_OR_EQUAL(type, name)                       \
clib_error_code_t AVLFunc(name, removeNearOrEqual) (                            \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value,                                                        \
    AVLNodeStruct(name) **a_res                                                 \
);

#define IMPLEMENT_AVL_FUNC_REMOVE_NEAR_OR_EQUAL(type, name)                     \
clib_error_code_t AVLFunc(name, removeNearOrEqual) (                            \
    AVLStruct(name) *a_avl,                                                     \
    const type *a_value,                                                        \
    AVLNodeStruct(name) **a_res)                                                \
{                                                                               \
    INPUT_CHECK(a_res == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER);                \
    *a_res = CLIB_NULLPTR;                                                      \
    INPUT_CHECK(a_avl == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER);                \
    INPUT_CHECK(a_value == CLIB_NULLPTR, CLIB_ERROR_NULL_POINTER);              \
                                                                                \
    clib_error_code_t err = CLIB_ERROR_SUCCESS;                                 \
    const AVLNodeStruct(name) *node = CLIB_NULLPTR;                             \
    do {                                                                        \
        err = AVLFunc(name, findNearOrEqual) (                                  \
            a_avl,                                                              \
            a_value,                                                            \
            &node                                                               \
        );                                                                      \
        if (err != CLIB_ERROR_SUCCESS) {                                        \
            break;                                                              \
        }                                                                       \
                                                                                \
        err = AVLFunc(name, remove) (                                           \
            a_avl,                                                              \
            &node->data,                                                        \
            a_res                                                               \
        );                                                                      \
    } while (0);                                                                \
                                                                                \
    return err;                                                                 \
}

#else // AVL_USE_REMOVE_NEAR_OR_EQUAL is not defined

#define DECLARE_AVL_FUNC_REMOVE_NEAR_OR_EQUAL(type, name)
#define IMPLEMENT_AVL_FUNC_REMOVE_NEAR_OR_EQUAL(type, name)

#endif // end if AVL_USE_REMOVE_NEAR_OR_EQUAL

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_REMOVE_NEAR_OR_EQUAL
