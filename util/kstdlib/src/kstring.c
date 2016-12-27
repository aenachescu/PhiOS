#include "types.h"
#include "kstring.h"
#include "errors.h"

size_t kmemchr(const void *str, sint32 c, size_t n, void **chr)
{
    if (str == NULL || chr == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (n != 0)
    {
        register cuint8 *p = str;
        do
        {
            if (*p++ == c)
            {
                *chr = (void*) (p - 1);
                return ERROR_SUCCESS;
            }
        } while (--n != 0);
    }
    else
    {
        return ERROR_INSUFFICIENT_BUFFER;
    }

    return ERROR_UNKNOWN;
}

size_t kmemcmp(const void *str1, const void *str2, size_t n, sint32 *ret)
{
    if (str1 == NULL || str2 == NULL || ret == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (n != 0)
    {
        register cuint8 *p1 = str1;
        register cuint8 *p2 = str2;
        do
        {
            if (*p1++ != *p2++)
            {
                *ret = *--p1 - *--p2;
                return ERROR_SUCCESS;
            }
        } while (--n != 0);
    }
    else
    {
        return ERROR_INSUFFICIENT_BUFFER;
    }

    return ERROR_UNKNOWN;
}

size_t kmemcpy(void *dest, const void *src, size_t n)
{
    if (dest == NULL || src == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (dest == src)
    {
        return ERROR_SAME_POINTERS;
    }

    if (n == 0)
    {
        return ERROR_INSUFFICIENT_BUFFER;
    }

    // TODO: optimize to use WORDS
    register char *pDest = (char*) dest;
    register char *pSrc = (char*) src;

    while (n--)
    {
        *pDest++ = *pSrc++;
    }

    return ERROR_SUCCESS;
}

size_t kmemmove(void *dest, const void *src, size_t n)
{
    if (dest == NULL || src == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (dest == src)
    {
        return ERROR_SAME_POINTERS;
    }

    if (n == 0)
    {
        return ERROR_INSUFFICIENT_BUFFER;
    }

    // TODO: optimize to use WORDS
    register char *pDest = dest;
    register const char *pSrc = src;

    // TODO: remove naive implementation for p1 < p2
    if (((pSrc) < (pDest)))
    {
        for (pDest += n, pSrc += n; n--;)
        {
            *--pDest = *--pSrc;
        }
    }
    else
    {
        while (n--)
        {
            *pDest++ = *pSrc++;
        }
    }

    return ERROR_SUCCESS;
}

size_t kmemset(void *str, sint32 c, size_t n)
{
    if (str == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (n == 0)
    {
        return ERROR_INSUFFICIENT_BUFFER;
    }

    // TODO: optimize to use WORDS
    register char *p = str;

    do
    {
        *p++ = c;
    } while (n--);

    return ERROR_SUCCESS;
}

size_t kstrcmp(csint8 *str1, csint8 *str2, sint32 *ret)
{
    if (str1 == NULL || str2 == NULL || ret == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (str1 == str2)
    {
        return ERROR_SAME_POINTERS;
    }

    register sint8 c1, c2;
    do
    {
        c1 = *str1++;
        c2 = *str2++;
        *ret = c1 - c2;
    } while ((*ret == 0) && (c1 != '\0'));

    return ERROR_SUCCESS;
}

size_t kstrcpy(char *a_dest, size_t *a_destLength, const char *a_src)
{
    if (a_dest == NULL || a_destLength == NULL || a_src == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_dest == a_src)
    {
        return ERROR_SAME_POINTERS;
    }

    register size_t length = 0;
    while (*a_src != '\0')
    {
        if (length < *a_destLength)
        {
            *a_dest = *a_src;
            ++a_dest;
        }

        ++a_src;
        ++length;
    }

    if (length < *a_destLength)
    {
        *a_dest = '\0';
        *a_destLength = length;
    }
    else
    {
        *a_destLength = length + 1;
        return ERROR_INSUFFICIENT_BUFFER;
    }

    return ERROR_SUCCESS;
}

size_t kstrlen(const char *a_str, size_t *a_length)
{
    if (a_str == NULL || a_length == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    *a_length = 0;
    while (*a_str != '\0')
    {
        (*a_length)++;
        a_str++;
    }

    return ERROR_SUCCESS;
}

size_t kstrrev(char *a_str, size_t a_length)
{
    if (a_str == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    size_t length = a_length;
    if (a_length == 0)
    {
        size_t error  = kstrlen(a_str, &length);

        if (error != ERROR_SUCCESS)
        {
            return error;
        }
    }

    register char *left = a_str;
    register char *right = a_str + (length - 1);
    register char tmp;

    while (left < right)
    {
        tmp = *left;
        *left = *right;
        *right = tmp;

        left++;
        right--;
    }

    return ERROR_SUCCESS;
}
