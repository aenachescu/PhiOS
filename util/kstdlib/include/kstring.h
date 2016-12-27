#ifndef PhiOS_kstring
#define PhiOS_kstring

#include "types.h"

void* kmemchr(const void *str, sint32 c, size_t n);
int kmemcmp(const void *str1, const void *str2, size_t n);
void* kmemcpy(void *dest, const void *src, size_t n);
void* kmemmove(void *dest, const void *src, size_t n);
void* kmemset(void *str, sint32 c, size_t n);
int kstrcmp(csint8 *str1, csint8 *str2);

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
