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

    bool extend = false;
    char ebuffer[64] = { 0 };
    uint64 eaddress;
    sint64 esvalue;
    uint64 euvalue;

    char tmp[3] = { 0 };
    tmp[0] = '%';

    ++args;

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
                        esvalue = *((sint64*) args);
                        args++;

                        bufferSize = 64;
                        ki64toa(esvalue, ebuffer, &bufferSize, 10);

                        VGA_WriteString(ebuffer);
                    }
                    else
                    {
                        svalue = *((sint32*)args);
                        args++;

                        bufferSize = 32;
                        kitoa(svalue, buffer, &bufferSize, 10);

                        VGA_WriteString(buffer);
                    }
                    extend = false;
                    break;

                case 'u':
                    if (extend)
                    {
                        euvalue = *((uint64*)args);
                        args++;

                        bufferSize = 64;
                        ku64toa(euvalue, ebuffer, &bufferSize, 10);

                        VGA_WriteString(ebuffer);
                    }
                    else
                    {
                        uvalue = *((uint32*)args);
                        args++;

                        bufferSize = 32;
                        kutoa(uvalue, buffer, &bufferSize, 10);

                        VGA_WriteString(buffer);
                    }
                    extend = false;
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
                    extend = false;
                    break;

                case 'c':
                    c = *((const char*)args);
                    args++;

                    VGA_WriteChar(c);
                    extend = false;
                    break;

                case 'p':
                    address = *((uint32*)args);
                    args++;

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
                        eaddress = *((uint64*)args);
                        args++;

                        bufferSize = 64;
                        kutoa(eaddress, ebuffer, &bufferSize, 16);

                        VGA_WriteChar('0');
                        VGA_WriteChar('x');

                        for (size_t i = 64 / 4 - bufferSize; i > 0; i--)
                        VGA_WriteChar('0');
                        VGA_WriteString(ebuffer);
                    }
                    else
                    {
                        address = *((size_t*)args);
                        args++;

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

    return ERROR_SUCCESS;
}
