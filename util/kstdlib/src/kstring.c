#include "types.h"
#include "kstring.h"
#include "errors.h"

void* kmemchr(const void *str, sint32 c, size_t n)
{
    if (n != 0)
    {
        register cuint8 *p = str;
        do
        {
            if (*p++ == c)
            {
                return ((void*) (p - 1));
            }
        } while (--n != 0);
    }

    return NULL;
}

int kmemcmp(const void *str1, const void *str2, size_t n)
{
    if (n != 0)
    {
        register cuint8 *p1 = str1;
        register cuint8 *p2 = str2;
        do
        {
            if (*p1++ != *p2++)
            {
                return (*--p1 - *--p2);
            }
        } while (-- n != 0);
    }

    return 0;
}

void* kmemcpy(void *dest, const void *src, size_t n)
{

}

void* kmemmove(void *dest, const void *src, size_t n)
{

}

void* kmemset(void *str, sint32 c, size_t n)
{

}

int kstrcmp(csint8 *str1, csint8 *str2)
{

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

    size_t length = 0;
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

    char *left = a_str;
    char *right = a_str + (length - 1);
    char tmp;

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

