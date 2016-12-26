#include "types.h"
#include "kstring.h"

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

char* kstrcpy(sint8 *dest, csint8 *src)
{

}

size_t kstrlen(csint8 *str)
{

}
