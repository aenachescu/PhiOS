#include "kstdio.h"
#include "vga/text_mode.h"
#include "errors.h"
#include "kstdlib.h"

size_t kprintf(const char *a_format, ...)
{
    if (a_format == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    char **args = (char**) &a_format;

    char c;
    const char *str = NULL;
    size_t address;
    sint32 svalue;
    uint32 uvalue;

    char buffer[32] = { 0 };
    size_t bufferSize = 32;

    char tmp[3] = { 0 };
    tmp[0] = '%';

    ++args;

    while ((c = *a_format) != '\0')
    {
        ++a_format;

        if (c == '%')
        {
            switch (*a_format)
            {
                case 'd':
                    svalue = *((sint32*)args);
                    args++;

                    bufferSize = 32;
                    kitoa(svalue, buffer, &bufferSize, 10);

                    VGA_WriteString(buffer);
                    break;

                case 'u':
                    uvalue = *((uint32*)args);
                    args++;

                    bufferSize = 32;
                    kutoa(uvalue, buffer, &bufferSize, 10);

                    VGA_WriteString(buffer);
                    break;

                case 's':
                    str = *args;
                    ++args;

                    if (str == NULL)
                    {
                        VGA_WriteString("(null)");
                    }
                    else
                    {
                        VGA_WriteString(str);
                    }
                    break;

                case 'c':
                    c = *((const char*)args);
                    args++;

                    VGA_WriteChar(c);
                    break;

                case 'p':
                    address = *((uint32*)args);

                    bufferSize = 32;
                    kutoa(address, buffer, &bufferSize, 16);

                    VGA_WriteChar('0');
                    VGA_WriteChar('X');
                    VGA_WriteString(buffer);
                    break;
                case 'x':
                    address = *((size_t*)args);

                    bufferSize = 32;
                    kutoa(address, buffer, &bufferSize, 16);

                    VGA_WriteChar('0');
                    VGA_WriteChar('x');

                    for (size_t i = WORDSIZE / 4 - bufferSize; i > 0; i--)
                        VGA_WriteChar('0');
                    VGA_WriteString(buffer);
                    break;

                default:
                    tmp[1] = *a_format;
                    VGA_WriteString(tmp);
            }
            ++a_format;
        }
        else
        {
            VGA_WriteChar(c);
        }
    }

    return ERROR_SUCCESS;
}
