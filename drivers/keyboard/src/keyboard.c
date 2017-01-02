#include "keyboard.h"
#include "cpu.h"
#include "types.h"
#include "errors.h"
#include "kstring.h"
#include "arch/x86/asm_io.h"
#include "kstdio.h"

char g_USasciiNonShift[] = {
0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE,
TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER, 0,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0,
KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
KHOME, KUP, KPGUP,'-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL,
0, 0, 0, KF11, KF12 };

char g_USasciiShift[] = {
0, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE,
TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ENTER, 0,
'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0,
KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
KHOME, KUP, KPGUP, '-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL,
0, 0, 0, KF11, KF12 };

char *g_layout;
char *g_shiftLayout;
uint8 g_keyboardBuffer[KEYBOARD_BUFFER_SIZE];
volatile uint32 g_keyboardBufferPos;
bool g_capsOn;

void helper_keyboardReadScanCode()
{
    if (g_keyboardBufferPos == KEYBOARD_BUFFER_SIZE) {
        kmemset(g_keyboardBuffer, 0, KEYBOARD_BUFFER_SIZE);
        g_keyboardBufferPos = 0;
    }

    uint8 code = io_inb(KEYBOARD_IO_PORT);
    bool special = false;
    bool shift = false;

    if (code > 88)
        return;

    if (code & 0x80)
    {
        code &= 0x7F;

        if (code == KRLEFT_SHIFT || code == KRRIGHT_SHIFT)
        {
            shift = false;
            special = true;
        }

            if (code == KRCAPS_LOCK)
        {
            g_capsOn = !g_capsOn;
        }
    }
    else
    {
        if (code == KRLEFT_SHIFT || code == KRRIGHT_SHIFT)
        {
            shift = true;
            special = true;
        }

        if (shift || g_capsOn)
        {
            g_keyboardBuffer[g_keyboardBufferPos++] = g_shiftLayout[code];
        }
        else
        {
            g_keyboardBuffer[g_keyboardBufferPos++] = g_layout[code];
        }

        if (special)
        {
            uint8 key = g_keyboardBuffer[--g_keyboardBufferPos];
            g_keyboardBuffer[g_keyboardBufferPos] = 0;

            // something useless to remove the 'unused' warn
            key -= key;
            key += key;
            // do special key work
        }
    }
}

size_t keyboard_init()
{
    size_t err = keyboard_setLayout(g_USasciiNonShift, g_USasciiShift);
    if (err != ERROR_SUCCESS)
    {
        return err;
    }

    kmemset(g_keyboardBuffer, 0, KEYBOARD_BUFFER_SIZE);
    g_keyboardBufferPos = 0;
    g_capsOn = false;

    return ERROR_SUCCESS;
}

size_t keyboard_setLayout(char *a_layout, char *a_shiftLayout)
{
    if (a_layout == NULL || a_shiftLayout == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    g_layout = a_layout;
    g_shiftLayout = a_shiftLayout;

    return ERROR_SUCCESS;
}

char keyboard_readKey()
{
    volatile uint8 cpos = g_keyboardBufferPos;

    while (true)
    {
        if (cpos != g_keyboardBufferPos &&
            g_keyboardBuffer[g_keyboardBufferPos - 1])
        {
            break;
        }
    }

    return (char) g_keyboardBuffer[g_keyboardBufferPos - 1];
}

void keyboard_intHandler32(__attribute__((unused)) IntCpuState32 *a_state)
{
    helper_keyboardReadScanCode();
}

/*void keyboard_intHandler64(__attribute__((unused)) IntCpuState64 *a_state)
{
    helper_keyboardReadScanCode();
}*/
