#ifndef PhiOS_kstring
#define PhiOS_kstring

#include "types.h"

void* kmemchr(const void *str, sint32 c, size_t n);
int kmemcmp(const void *str1, const void *str2, size_t n);
void* kmemcpy(void *dest, const void *src, size_t n);
void* kmemmove(void *dest, const void *src, size_t n);
void* kmemset(void *str, sint32 c, size_t n);
int kstrcmp(csint8 *str1, csint8 *str2);
char* kstrcpy(sint8 *dest, csint8 *src);
size_t kstrlen(csint8 *str);

#endif
