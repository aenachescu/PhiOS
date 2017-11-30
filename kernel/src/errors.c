#include "include/errors.h"
#include "include/types.h"

void __kernel_check(size_t err, const char *file, size_t line)
{
    (void)file;
    (void)line;
    (void)err;
}
