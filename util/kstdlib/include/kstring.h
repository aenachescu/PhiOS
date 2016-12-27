#ifndef PhiOS_kstring
#define PhiOS_kstring

#include "types.h"

size_t kmemchr(const void *a_buffer, uint8 a_value,
               size_t a_length, void **a_result);

size_t kmemcmp(const void *a_buffer1, const void *a_buffer2,
               size_t a_length, sint32 *a_result);

size_t kmemcpy(void *a_dest, const void *a_src, size_t a_length);

size_t kmemmove(void *a_dest, const void *a_src, size_t a_length);

size_t kmemset(void *a_buffer, sint8 a_value, size_t a_length);

size_t kstrcmp(const char *a_str1, const char *a_str2, sint32 *a_result);

/*
 * @brief Copies a_src into a_dest.
 *
 * @param a_dest Pointer to the destination where the content will be copied.
 * @param a_destLength The length of a_dest.
 * @param a_src The string which will be copied.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends successfully and in a_destLength will
 *      be stored the string length.
 *  ERROR_INSUFFICIENT_BUFFER - if the length of a_dest is too small and in
 *      a_destLength will be stored the required size.
 *  ERROR_SAME_POINTER - if a_dest and a_src are equal (as addresses, not as
 *      strings).
 *  ERROR_NULL_POINTER - if at least one pointer is null.
 */
size_t kstrcpy(char *a_dest, size_t *a_destLength, const char *a_src);

/*
 * @brief Computes the string length.
 *
 * @param a_str Pointer to the string for which will be computed the length.
 * @param a_length Pointer to size_t, where will be saved the string length.
 *
 * @return
 *  ERROR_SUCCESS       - if the function ends successfully
 *  ERROR_POINTER_NULL  - if a_str is null
 */
size_t kstrlen(const char *a_str, size_t *a_length);

/*
 * @brief Reverse the string
 *
 * @param a_str Pointer to the string that must be reversed.
 * @param a_length The string length. If is 0 then will be called strlen.
 *
 * @return
 *  ERROR_SUCCESS       - if the function ends successfully
 *  ERROR_POINTER_NULL  - if a_str is null
 */
size_t kstrrev(char *a_str, size_t a_length);

#endif
