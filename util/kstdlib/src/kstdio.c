#include "kstdio.h"
#include "vga/text_mode.h"
#include "errors.h"
#include "kstdlib.h"

#include <stdarg.h>

size_t kprintf(const char *a_format, ...)
{
    if (a_format == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    char c;
    const char *str = NULL;
    size_t address;
    sint32 svalue;
    uint32 uvalue;

    char buffer[32] = { 0 };
    size_t bufferSize = 32;

    bool extend = false;
    char ebuffer[64] = { 0 };
    uint64 eaddress;
    sint64 esvalue;
    uint64 euvalue;

    char tmp[3] = { 0 };
    tmp[0] = '%';


    va_list arg;
    va_start(arg, a_format);

    while ((c = *a_format) != '\0')
    {
        ++a_format;

        if (c == '%' || extend)
        {
            switch (*a_format)
            {
                case 'd':
                    if (extend)
                    {
                        esvalue = va_arg(arg, sint64);

                        bufferSize = 64;
                        ki64toa(esvalue, ebuffer, &bufferSize, 10);

                        VGA_WriteString(ebuffer);
                    }
                    else
                    {
                        svalue = va_arg(arg, sint32);

                        bufferSize = 32;
                        kitoa(svalue, buffer, &bufferSize, 10);

                        VGA_WriteString(buffer);
                    }
                    extend = false;
                    break;

                case 'u':
                    if (extend)
                    {
                        euvalue = va_arg(arg, uint64);

                        bufferSize = 64;
                        ku64toa(euvalue, ebuffer, &bufferSize, 10);

                        VGA_WriteString(ebuffer);
                    }
                    else
                    {
                        uvalue = va_arg(arg, uint32);

                        bufferSize = 32;
                        kutoa(uvalue, buffer, &bufferSize, 10);

                        VGA_WriteString(buffer);
                    }
                    extend = false;
                    break;

                case 's':
                    str = va_arg(arg, const char*);

                    if (str == NULL)
                    {
                        VGA_WriteString("(null)");
                    }
                    else
                    {
                        VGA_WriteString(str);
                    }
                    extend = false;
                    break;

                case 'c':
                    c = va_arg(arg, int);

                    VGA_WriteChar(c);
                    extend = false;
                    break;

                case 'p':
                    address = va_arg(arg, uint32);

                    bufferSize = 32;
                    kutoa(address, buffer, &bufferSize, 16);

                    VGA_WriteChar('0');
                    VGA_WriteChar('x');
                    VGA_WriteString(buffer);
                    extend = false;
                    break;

                case 'x':
                    if (extend)
                    {
                        eaddress = va_arg(arg, uint64);

                        bufferSize = 64;
                        ku64toa(eaddress, ebuffer, &bufferSize, 16);

                        VGA_WriteChar('0');
                        VGA_WriteChar('x');

                        for (size_t i = 64 / 4 - bufferSize; i > 0; i--)
                        VGA_WriteChar('0');
                        VGA_WriteString(ebuffer);
                    }
                    else
                    {
                        address = va_arg(arg, uint32);

                        bufferSize = 32;
                        kutoa(address, buffer, &bufferSize, 16);

                        VGA_WriteChar('0');
                        VGA_WriteChar('x');

                        for (size_t i = WORDSIZE / 4 - bufferSize; i > 0; i--)
                        VGA_WriteChar('0');
                        VGA_WriteString(buffer);
                    }
                    extend = false;
                    break;

                case 'l':
                    if (*(a_format + 1) == 'l') 
                    {
                        extend = true;
                    }
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

    va_end(arg);

    return ERROR_SUCCESS;
}
