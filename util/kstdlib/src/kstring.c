#include "types.h"
#include "kstring.h"
#include "errors.h"

uint32 kmemchr(
    const void *a_buffer, 
    uint8 a_value,
    size_t a_length, 
    void **a_result)
{
    if (a_buffer == NULL || a_result == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    *a_result = NULL;

    if (a_length != 0)
    {
        register cuint8 *p = (cuint8*) a_buffer;
        while (a_length != 0)
        {
            if (*p == a_value)
            {
                *a_result = (void*) p;
                return ERROR_SUCCESS;
            }

            --a_length;
            ++p;
        }
    }
    else
    {
        return ERROR_EMPTY_BUFFER;
    }

    return ERROR_NOT_FOUND;
}

uint32 kmemcmp(
    const void *a_buffer1, 
    const void *a_buffer2,
    size_t a_length, 
    sint32 *a_result)
{
    if (a_buffer1 == NULL || a_buffer2 == NULL || a_result == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_buffer1 == a_buffer2)
    {
        return ERROR_SAME_POINTERS;
    }

    if (a_length != 0)
    {
        register cuint8 *p1 = a_buffer1;
        register cuint8 *p2 = a_buffer2;
        while (a_length != 0)
        {
            if ((*p1) != (*p2))
            {
                *a_result = (*p1) - (*p2);
                return ERROR_SUCCESS;
            }

            p1++;
            p2++;
            a_length--;
        }

        *a_result = 0;
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_EMPTY_BUFFER;
    }

    return ERROR_UNKNOWN;
}

uint32 kmemcpy(
    void *a_dest, 
    const void *a_src, 
    size_t a_length)
{
    if (a_dest == NULL || a_src == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_dest == a_src)
    {
        return ERROR_SAME_POINTERS;
    }

    if (a_length == 0)
    {
        return ERROR_INSUFFICIENT_BUFFER;
    }

    // TODO: optimize to use WORDS
    register char *pDest = (char*) a_dest;
    register char *pSrc = (char*) a_src;

    while (a_length != 0)
    {
        *pDest = *pSrc;
        pDest++;
        pSrc++;
        a_length--;
    }

    return ERROR_SUCCESS;
}

uint32 kmemmove(
    void *a_dest, 
    const void *a_src, 
    size_t a_length)
{
    if (a_dest == NULL || a_src == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_dest == a_src)
    {
        return ERROR_SAME_POINTERS;
    }

    if (a_length == 0)
    {
        return ERROR_EMPTY_BUFFER;
    }

    // TODO: optimize to use WORDS
    register char *pDest = a_dest;
    register const char *pSrc = a_src;

    // TODO: remove naive implementation for p1 < p2
    if (((pSrc) < (pDest)))
    {
        pSrc  += a_length;
        pDest += a_length;

        while (a_length != 0)
        {
            *pDest = *pSrc;
            pDest--;
            pSrc--;
            a_length--;
        }
    }
    else
    {
        while (a_length != 0)
        {
            *pDest = *pSrc;
            pDest++;
            pSrc++;
            a_length--;
        }
    }

    return ERROR_SUCCESS;
}

uint32 kmemset(
    void *a_buffer, 
    sint8 a_value, 
    size_t a_length)
{
    if (a_buffer == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_length == 0)
    {
        return ERROR_EMPTY_BUFFER;
    }

    // TODO: optimize to use WORDS
    register char *p = a_buffer;

    do
    {
        *p = a_value;
        p++;
        a_length--;
    } while (a_length != 0);

    return ERROR_SUCCESS;
}

uint32 kstrcmp(
    const char *a_str1, 
    const char *a_str2, 
    sint32 *a_result)
{
    if (a_str1 == NULL || a_str2 == NULL || a_result == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_str1 == a_str2)
    {
        return ERROR_SAME_POINTERS;
    }

    *a_result = 0;
    while (*a_result == 0 && *a_str1 != '\0' && *a_str2 != '\0')
    {
        *a_result = (*a_str1) - (*a_str2);
        a_str1++;
        a_str2++;
    }

    if (*a_result == 0 && (*a_str1) != (*a_str2))
    {
        if (*a_str1 == '\0')
        {
            *a_result = -1;
        }

        if (*a_str2 == '\0')
        {
            *a_result = 1;
        }
    }

    return ERROR_SUCCESS;
}

uint32 kstrcpy(
    char *a_dest, 
    size_t *a_destLength, 
    const char *a_src)
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

uint32 kstrlen(
    const char *a_str, 
    size_t *a_length)
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

uint32 kstrrev(
    char *a_str, 
    size_t a_length)
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
