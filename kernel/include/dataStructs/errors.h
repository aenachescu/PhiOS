#ifndef CLIB_TEMPLATE_DATA_STRUCTURES_ERRORS
#define CLIB_TEMPLATE_DATA_STRUCTURES_ERRORS

typedef unsigned int clib_error_code_t;

#define CLIB_ERROR_SUCCESS           ((clib_error_code_t) 0)
#define CLIB_ERROR_NULL_POINTER      ((clib_error_code_t) 1)
#define CLIB_ERROR_NOT_FOUND         ((clib_error_code_t) 2)
#define CLIB_ERROR_NO_FREE_MEMORY    ((clib_error_code_t) 3)
#define CLIB_ERROR_INTERNAL_ERROR    ((clib_error_code_t) 4)
#define CLIB_ERROR_UNINITIALIZED     ((clib_error_code_t) 5)
#define CLIB_ERROR_INVALID_FUNCTION  ((clib_error_code_t) 6)

#endif // end if CLIB_TEMPLATE_DATA_STRUCTURES_ERRORS
