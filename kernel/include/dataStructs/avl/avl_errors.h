#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_AVL_ERRORS
#define CLIB_TEMPLATE_DATA_STRUCTURES_AVL_ERRORS

typedef unsigned int avl_error_code_t;

#define AVL_ERROR_SUCCESS           ((avl_error_code_t) 0)
#define AVL_ERROR_NULL_POINTER      ((avl_error_code_t) 1)
#define AVL_ERROR_NOT_FOUND         ((avl_error_code_t) 2)
#define AVL_ERROR_NO_FREE_MEMORY    ((avl_error_code_t) 3)
#define AVL_ERROR_INTERNAL_ERROR    ((avl_error_code_t) 4)
#define AVL_ERROR_UNINITIALIZED     ((avl_error_code_t) 5)

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_AVL_ERRORS
