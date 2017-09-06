#include "kernel/include/logging.h"
#include "include/types.h"

#include <stdarg.h>

#include "drivers/video/include/vga/text_mode.h"

#include "util/kstdlib/include/kstdlib.h"

#include "include/cpu.h"
#include "drivers/serial/include/serial.h"

#define BUFFER_SIZE 512
#define TMP_BUFF_LENGTH 65

#define LOG_TYPE_MAX_LENGTH 8
#define LOG_LINE_MAX_LENGTH 6

#define LENGTH_none 0
#define LENGTH_hh   1
#define LENGTH_h    2
#define LENGTH_l    3
#define LENGTH_ll   4
#define LENGTH_j    5
#define LENGTH_z    6
#define LENGTH_t    7
#define LENGTH_L    8

#define IS_DIGIT(x) (x >= '0' && x <= '9')

#define DEFAULT_CASES       \
    case '%':               \
        goto reinit;        \
    case '\0':              \
        goto end;           \
    default:                \
        exit = true;

#define REINIT              \
    backupFormat = NULL;    \
    ++a_format;             \
    goto reinit

static inline void write(
    const char *str)
{
    VGA_WriteString(str);
    serial_writeString(str, SERIAL_PORT_A);
}

static inline void addChar(
    char *res,
    char c,
    uint32 *pos)
{
    if (*pos == BUFFER_SIZE - 1) {
        res[*pos] = 0;
        write(res);
        *pos = 0;
    }
    res[*pos] = c;
    (*pos)++;
}

static inline void addChars(
    char *res,
    char c,
    uint32 num,
    uint32 *pos)
{
    while (true) {
        while (*pos < BUFFER_SIZE - 1 && num > 0) {
            res[*pos] = c;
            --num;
            ++(*pos);
        }

        if (num == 0) {
            return;
        }

        res[*pos] = 0;
        write(res);
        *pos = 0;
    }
}

static inline void addString(
    char *res,
    const char *str,
    uint32 *pos)
{
    while (true) {
        while (*pos < BUFFER_SIZE - 1 && *str != '\0') {
            res[*pos] = *str;
            str++;
            (*pos)++;
        }

        if (*str == '\0') {
            return;
        }

        res[*pos] = 0;
        write(res);
        *pos = 0;
    }
}

static inline void intToStr(
    sint32 val,
    char *str,
    size_t *length,
    size_t base,
    bool flag_space,
    bool flag_plus)
{
    if ((flag_space == false && flag_plus == false) || val < 0) {
        kitoa(val, str, length, base);
        return;
    }

    if (flag_space == true) {
        str[0] = ' ';
    } else if (flag_plus == true) {
        str[0] = '+';
    }

    --(*length);
    kitoa(val, str + 1, length, base);
    ++(*length);
}

static inline void int64ToStr(
    sint64 val,
    char *str,
    size_t *length,
    size_t base,
    bool flag_space,
    bool flag_plus)
{
    if ((flag_space == false && flag_plus == false) || val < 0) {
        ki64toa(val, str, length, base);
        return;
    }

    if (flag_space == true) {
        str[0] = ' ';
    } else if (flag_plus == true) {
        str[0] = '+';
    }

    --(*length);
    ki64toa(val, str + 1, length, base);
    ++(*length);
}

static inline void hexToLower(
    char *str)
{
    register char *p = str;
    while (*p != 0) {
        if (*p >= 'A' && *p <= 'Z') {
            (*p) += 32;
        }
        p++;
    }
}

void __klog(
    const char *a_file,
    int a_line,
    const char *a_logType,
    const char *a_format,
    ...)
{
    if (a_format == NULL ||
        a_file == NULL ||
        a_logType == NULL ||
        a_line < 0) {
        return;
    }

    char result[BUFFER_SIZE] = { 0 };
    char tmpBuff[TMP_BUFF_LENGTH] = { 0 };
    size_t tmpBuffLength = TMP_BUFF_LENGTH;
    uint32 currentPos = 0;
    const char *backupFormat = NULL;
    char ch;
    bool exit;

    bool flag_minus, flag_plus, flag_space, flag_zero, flag_hash;
    bool processingFormat;
    uint32 width, precision, length;
    bool widthAsterix, precisionAsterix;

    uint32 uval32;
    uint64 uval64;
    sint32 val32;
    sint64 val64;

    va_list arg;
    va_start(arg, a_format);

reinit:
    processingFormat = false;
    widthAsterix = false;
    precisionAsterix = false;
    flag_minus = false;
    flag_plus = false;
    flag_space = false;
    flag_zero = false;
    flag_hash = false;
    width = 0;
    precision = 0;
    length = LENGTH_none;
    tmpBuffLength = TMP_BUFF_LENGTH;

    if (backupFormat != NULL) {
        while (backupFormat <= a_format) {
            addChar(result, *backupFormat, &currentPos);
            backupFormat++;
        }

        backupFormat = NULL;
        a_format++;
    }

    while (*a_format != '\0') {
        // backup for pointer if the format does not meet the standard
        backupFormat = a_format;

        if (*a_format != '%') {
            addChar(result, *a_format, &currentPos);
            ++a_format;
            continue;
        }

        ++a_format;

        // for %% case
        if (*a_format == '%') {
            addChar(result, *a_format, &currentPos);
            ++a_format;
            continue;
        }

        processingFormat = true;

        // processing flags
        exit = false;
        while (true) {
            switch (*a_format) {
                case '-':
                    flag_minus = true;
                    break;

                case '+':
                    flag_plus = true;
                    break;

                case ' ':
                    flag_space = true;
                    break;

                case '#':
                    flag_hash = true;
                    break;

                case '0':
                    flag_zero = true;
                    break;

                DEFAULT_CASES;
            }

            if (exit == true) {
                break;
            }

            ++a_format;
        }

        // processing width
        exit = false;
        while (true) {
            switch (*a_format) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    width *= 10;
                    width += (*a_format - '0');
                    break;

                case '*':
                    widthAsterix = true;
                    ++a_format;
                    exit = true;
                    break;

                DEFAULT_CASES;
            }

            if (exit == true) {
                break;
            }

            ++a_format;
        }

        // processing precision
        if (*a_format != '.') {
            goto skipPrecision;
        }
        ++a_format;
        exit = false;
        while (true) {
            switch (*a_format) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    precision *= 10;
                    precision += (*a_format - '0');
                    break;

                case '*':
                    precisionAsterix = true;
                    ++a_format;
                    exit = true;
                    break;

                DEFAULT_CASES;
            }

            if (exit == true) {
                break;
            }

            ++a_format;
        }
skipPrecision:

        // processing length
        exit = false;
        switch (*a_format) {
            case 'h':
                if (a_format[1] == 'h') {
                    ++a_format;
                    length = LENGTH_hh;
                } else {
                    length = LENGTH_h;
                }
                break;

            case 'l':
                if (a_format[1] == 'l') {
                    ++a_format;
                    length = LENGTH_ll;
                } else {
                    length = LENGTH_l;
                }
                break;

            case 'j':
                length = LENGTH_j;
                break;

            case 'z':
                length = LENGTH_z;
                break;

            case 't':
                length = LENGTH_t;
                break;

            case 'L':
                length = LENGTH_L;
                break;

            DEFAULT_CASES;
        }

        if (exit == false) {
            ++a_format;
        }

        // processing specifiers
        exit = false;
        switch (*a_format) {
            case 'd':
            case 'i':
                if (widthAsterix == true) {
                    width = va_arg(arg, uint32);
                }

                switch (length) {
                    case LENGTH_L:
                        // interpreted as int32
                    case LENGTH_l:
                    case LENGTH_none:
                        // int32
                        val32 = va_arg(arg, sint32);
                        intToStr(val32, tmpBuff, &tmpBuffLength, 10, flag_space, flag_plus);
                        break;

                    case LENGTH_j:
                        // intmax_t - interpreted as int64
                    case LENGTH_ll:
                        // int64
                        val64 = va_arg(arg, sint64);
                        int64ToStr(val64, tmpBuff, &tmpBuffLength, 10, flag_space, flag_plus);
                        break;

                    case LENGTH_h:
                        // int16
                        val32 = (sint16) va_arg(arg, sint32);
                        intToStr(val32, tmpBuff, &tmpBuffLength, 10, flag_space, flag_plus);
                        break;

                    case LENGTH_hh:
                        // int8
                        val32 = (sint8) va_arg(arg, sint32);
                        intToStr(val32, tmpBuff, &tmpBuffLength, 10, flag_space, flag_plus);
                        break;

                    case LENGTH_t:
                        // ptrdiff_t - interpreted as int32 on 32bits and int64 on 64bits
                    case LENGTH_z:
                        // size_t
#if defined WORDSIZE && WORDSIZE == 64
                        val64 = va_arg(arg, sint64);
                        int64ToStr(val64, tmpBuff, &tmpBuffLength, 10, flag_space, flag_plus);
#else
                        val32 = va_arg(arg, sint32);
                        intToStr(val32, tmpBuff, &tmpBuffLength, 10, flag_space, flag_plus);
#endif
                        break;
                }

                if (tmpBuffLength < width) {
                    if (flag_minus == true) {
                        addString(result, tmpBuff, &currentPos);
                        addChars(result, ' ', width - tmpBuffLength, &currentPos);
                    } else {
                        if (flag_zero == true) {
                            if (IS_DIGIT(tmpBuff[0])) {
                                addChars(result, '0', width - tmpBuffLength, &currentPos);
                                addString(result, tmpBuff, &currentPos);
                            } else {
                                addChar(result, tmpBuff[0], &currentPos);
                                addChars(result, '0', width - tmpBuffLength, &currentPos);
                                addString(result, tmpBuff + 1, &currentPos);
                            }
                        } else {
                            addChars(result, ' ', width - tmpBuffLength, &currentPos);
                            addString(result, tmpBuff, &currentPos);
                        }
                    }
                } else {
                    addString(result, tmpBuff, &currentPos);
                }

                REINIT;

            case 'u':
                REINIT;

            case 'o':
                // TODO: add support for this specifier
                REINIT;

            case 'x':
            case 'X':
                REINIT;

            case 'f':
            case 'F':
                // TODO: add support for these specifiers
                REINIT;

            case 'e':
            case 'E':
                // TODO: add support for these specifiers
                REINIT;

            case 'g':
            case 'G':
                // TODO: add support for these specifiers
                REINIT;

            case 'a':
            case 'A':
                // TODO: add support for these specifiers
                REINIT;

            case 'p':
            case 'P':
#if defined WORDSIZE && WORDSIZE == 64
                uval64 = va_arg(arg, uint64);
                ku64toa(uval64, tmpBuff, &tmpBuffLength, 16);
#else
                uval32 = va_arg(arg, uint32);
                kutoa(uval32, tmpBuff, &tmpBuffLength, 16);
#endif

                if (*a_format == 'p') {
                    hexToLower(tmpBuff);
                    addString(result, "0x", &currentPos);
                } else {
                    addString(result, "0X", &currentPos);
                }

                if (flag_hash == false) {
                    addChars(result, '0', WORDSIZE_BYTES * 2 - tmpBuffLength, &currentPos);
                }
                addString(result, tmpBuff, &currentPos);

                REINIT;

            case 'c':
                ch = (char) va_arg(arg, int);
                addChar(result, ch, &currentPos);
                REINIT;

            case 's':
                REINIT;

            case 'n':
                // interpreted as invalid...for now...
                goto reinit;

            DEFAULT_CASES;
        }

        goto reinit;
    }

end:

    va_end(arg);

    if (processingFormat == true && backupFormat != NULL) {
        while (backupFormat <= a_format) {
            addChar(result, *backupFormat, &currentPos);
            backupFormat++;
        }
    }

    result[currentPos] = 0;
    write(result);
}
