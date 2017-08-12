#include "kstdlib.h"
#include "errors.h"
#include "kstring.h"

size_t kitoa(sint32 a_value, char *a_buffer, size_t *a_length, size_t a_base)
{
    if (a_buffer == NULL || a_length == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_value < 0 && a_base == 10)
    {
        size_t length = *a_length;

        if (length > 0)
        {
            a_buffer[0] = '-';
            length--;
            a_buffer++;
        }

        size_t error = kutoa(-a_value, a_buffer, &length, a_base);
        *a_length = length + 1; // +1 from '-'

        return error;
    }
    else
    {
        return kutoa(a_value, a_buffer, a_length, a_base);
    }

    return ERROR_SUCCESS;
}

size_t kutoa(uint32 a_value, char *a_buffer, size_t *a_length, size_t a_base)
{
    if (a_buffer == NULL || a_length == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    size_t length = 0;
    char hexa[] = "0123456789ABCDEF";

    switch (a_base)
    {
        case 2:
            for (size_t i = 0; i < sizeof(uint64) * 8; i++)
            {
                if (length < (*a_length))
                {
                    a_buffer[length] = (a_value & (1 << i));
                }

                length++;
            }
            break;

        case 10:
            do
            {
                if (length < (*a_length))
                {
                    a_buffer[length] = '0' + (a_value % 10);
                }

                a_value /= 10;
                length++;
            } while (a_value);
            break;

        case 16:
            do
            {
                if (length < (*a_length))
                {
                    a_buffer[length] = hexa[a_value % 16];
                }

                a_value /= 16;
                length++;
            } while (a_value);
            break;

        default:
            return ERROR_UNSUPPORTED;
    }

    if (length < (*a_length))
    {
        a_buffer[length] = '\0';
        kstrrev(a_buffer, length);
        *a_length = length;
    }
    else
    {
        *a_length = length + 1;
        return ERROR_INSUFFICIENT_BUFFER;
    }

    return ERROR_SUCCESS;
}

size_t ki64toa(sint64 a_value, char *a_buffer, size_t *a_length, size_t a_base)
{
    if (a_buffer == NULL || a_length == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_value < 0 && a_base == 10)
    {
        size_t length = *a_length;

        if (length > 0)
        {
            a_buffer[0] = '-';
            length--;
            a_buffer++;
        }

        size_t error = kutoa(-a_value, a_buffer, &length, a_base);
        *a_length = length + 1; // +1 from '-'

        return error;
    }
    else
    {
        return kutoa(a_value, a_buffer, a_length, a_base);
    }

    return ERROR_SUCCESS;
}

size_t ku64toa(uint64 a_value, char *a_buffer, size_t *a_length, size_t a_base)
{
    if (a_buffer == NULL || a_length == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    size_t length = 0;
    char hexa[] = "0123456789ABCDEF";

    switch (a_base)
    {
        case 2:
            for (size_t i = 0; i < sizeof(uint64) * 8; i++)
            {
                if (length < (*a_length))
                {
                    a_buffer[length] = (a_value & (1 << i));
                }

                length++;
            }
            break;

        case 10:
            do
            {
                if (length < (*a_length))
                {
                    a_buffer[length] = '0' + (a_value % 10);
                }

                a_value /= 10;
                length++;
            } while (a_value);
            break;

        case 16:
            do
            {
                if (length < (*a_length))
                {
                    a_buffer[length] = hexa[a_value % 16];
                }

                a_value /= 16;
                length++;
            } while (a_value);
            break;

        default:
            return ERROR_UNSUPPORTED;
    }

    if (length < (*a_length))
    {
        a_buffer[length] = '\0';
        kstrrev(a_buffer, length);
        *a_length = length;
    }
    else
    {
        *a_length = length + 1;
        return ERROR_INSUFFICIENT_BUFFER;
    }

    return ERROR_SUCCESS;
}
