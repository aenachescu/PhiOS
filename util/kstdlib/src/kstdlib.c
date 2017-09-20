#include "util/kstdlib/include/kstdlib.h"
#include "util/kstdlib/include/kstring.h"

#include "kernel/include/random.h"

uint32 kitoa(
    sint32 a_value,
    char *a_buffer,
    size_t *a_length,
    size_t a_base)
{
    if (a_buffer == NULL || a_length == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_value < 0 && a_base == 10) {
        size_t length = *a_length;
        if (length > 0) {
            a_buffer[0] = '-';
            length--;
            a_buffer++;
        }

        uint32 error = kutoa(-a_value, a_buffer, &length, a_base);
        *a_length = length + 1; // +1 from '-'

        return error;
    } else {
        return kutoa(a_value, a_buffer, a_length, a_base);
    }

    return ERROR_SUCCESS;
}

uint32 kutoa(
    uint32 a_value,
    char *a_buffer,
    size_t *a_length,
    size_t a_base)
{
    if (a_buffer == NULL || a_length == NULL) {
        return ERROR_NULL_POINTER;
    }

    size_t length = 0;
    char hexa[] = "0123456789ABCDEF";

    switch (a_base) {
        case 2:
            for (size_t i = 0; i < sizeof(uint32) * 8; i++) {
                if (length < (*a_length)) {
                    if (a_value & (1 << i)) {
                        a_buffer[length] = '1';
                    } else {
                        a_buffer[length] = '0';
                    }
                }

                length++;
            }
            break;

        case 10:
            do {
                if (length < (*a_length)) {
                    a_buffer[length] = '0' + (a_value % 10);
                }

                a_value /= 10;
                length++;
            } while (a_value);
            break;

        case 16:
            do {
                if (length < (*a_length)) {
                    a_buffer[length] = hexa[a_value % 16];
                }

                a_value /= 16;
                length++;
            } while (a_value);
            break;

        default:
            return ERROR_UNSUPPORTED;
    }

    if (length < (*a_length)) {
        a_buffer[length] = '\0';
        kstrrev(a_buffer, length);
        *a_length = length;
    } else {
        a_buffer[*a_length - 1] = '\0';
        *a_length = length + 1;
        return ERROR_INSUFFICIENT_BUFFER;
    }

    return ERROR_SUCCESS;
}

uint32 ki64toa(
    sint64 a_value,
    char *a_buffer,
    size_t *a_length,
    size_t a_base)
{
    if (a_buffer == NULL || a_length == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_value < 0 && a_base == 10) {
        size_t length = *a_length;
        if (length > 0) {
            a_buffer[0] = '-';
            length--;
            a_buffer++;
        }

        size_t error = ku64toa(-a_value, a_buffer, &length, a_base);
        *a_length = length + 1; // +1 from '-'

        return error;
    } else {
        return ku64toa(a_value, a_buffer, a_length, a_base);
    }

    return ERROR_SUCCESS;
}

uint32 ku64toa(
    uint64 a_value,
    char *a_buffer,
    size_t *a_length,
    size_t a_base)
{
    if (a_buffer == NULL || a_length == NULL) {
        return ERROR_NULL_POINTER;
    }

    size_t length = 0;
    char hexa[] = "0123456789ABCDEF";

    switch (a_base) {
        case 2:
            for (size_t i = 0; i < sizeof(uint64) * 8; i++) {
                if (length < (*a_length)) {
                    if (a_value & (((uint64) 1) << i)) {
                        a_buffer[length] = '1';
                    } else {
                        a_buffer[length] = '0';
                    }
                }

                length++;
            }
            break;

        case 10:
            do {
                if (length < (*a_length)) {
                    a_buffer[length] = '0' + (a_value % 10);
                }

                a_value /= 10;
                length++;
            } while (a_value);
            break;

        case 16:
            do {
                if (length < (*a_length)) {
                    a_buffer[length] = hexa[a_value % 16];
                }

                a_value /= 16;
                length++;
            } while (a_value);
            break;

        default:
            return ERROR_UNSUPPORTED;
    }

    if (length < (*a_length)) {
        a_buffer[length] = '\0';
        kstrrev(a_buffer, length);
        *a_length = length;
    } else {
        a_buffer[*a_length - 1] = '\0';
        *a_length = length + 1;
        return ERROR_INSUFFICIENT_BUFFER;
    }

    return ERROR_SUCCESS;
}

static size_t g_seed;

uint32 krand(
    size_t *a_value)
{
    if (a_value == NULL) {
        return ERROR_NULL_POINTER;
    }

    size_t value;

    value = (((g_seed * 214013L + 2531011L) >> 16) & 0x7fff);
    g_seed = (((g_seed * 310412 + 1101352L) >> 16) & 0x7fff);

    *a_value = value;
    return ERROR_SUCCESS;
}

uint32 kranduint(
    size_t *a_value,
    size_t a_start,
    size_t a_end)
{
    if (a_value == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_start >= a_end) {
        return ERROR_INVALID_PARAMETER;
    }

    krand(a_value);

    // TODO: implement this

    return ERROR_SUCCESS;
}

uint32 ksrand(
    uint64 a_seed)
{
    g_seed = a_seed ^ kernel_random32();

    return ERROR_SUCCESS;
}