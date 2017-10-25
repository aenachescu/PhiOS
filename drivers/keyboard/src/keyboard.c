#include "drivers/keyboard/include/keyboard.h"

#include "util/kstdlib/include/kstring.h"

#include "kernel/include/arch/x86/asm_io.h"
#include "kernel/include/arch/x86/idt.h"

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

char g_USasciiCapsOn[] = {
0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE,
TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', ENTER, 0,
'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\',
'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, 0, 0, ' ', 0,
KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
KHOME, KUP, KPGUP,'-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL,
0, 0, 0, KF11, KF12 };

char *g_layout;
char *g_shiftLayout;
char *g_capsOnLayout;
uint8 g_keyboardBuffer[KEYBOARD_BUFFER_SIZE];
volatile uint32 g_keyboardBufferPos;
bool g_capsOn;
bool g_shift;

void helper_keyboardReadScanCode()
{
    if (g_keyboardBufferPos == KEYBOARD_BUFFER_SIZE) {
        kmemset(g_keyboardBuffer, 0, KEYBOARD_BUFFER_SIZE);
        g_keyboardBufferPos = 0;
    }

    uint8 code = io_inb(KEYBOARD_IO_PORT);

    if (code & 0x80) {
        code &= 0x7F;

        if (code == KRLEFT_SHIFT || code == KRRIGHT_SHIFT) {
            g_shift = false;
        }
    } else {
        if (code == KRLEFT_SHIFT || code == KRRIGHT_SHIFT) {
            g_shift = true;
            return;
        }

        if (code == KRCAPS_LOCK) {
            g_capsOn = !g_capsOn;
            return;
        }

        if (g_shift) {
            g_keyboardBuffer[g_keyboardBufferPos++] = g_shiftLayout[code];
        } else if (g_capsOn) {
            g_keyboardBuffer[g_keyboardBufferPos++] = g_capsOnLayout[code];
        } else {
            g_keyboardBuffer[g_keyboardBufferPos++] = g_layout[code];
        }
    }
}

uint32 keyboard_init()
{
    uint32 err = keyboard_setLayout(
        g_USasciiNonShift,
        g_USasciiShift,
        g_USasciiCapsOn
    );
    if (err != ERROR_SUCCESS) {
        return err;
    }

    kmemset(g_keyboardBuffer, 0, KEYBOARD_BUFFER_SIZE);
    g_keyboardBufferPos = 0;

    g_capsOn = false;
    g_shift = false;

    IDT_registerHandler(IRQ1, &keyboard_intHandler);

    return ERROR_SUCCESS;
}

uint32 keyboard_setLayout(
    char *a_layout,
    char *a_shiftLayout,
    char *a_capsOnLayout)
{
    if (a_layout == NULL || a_shiftLayout == NULL || a_capsOnLayout == NULL) {
        return ERROR_NULL_POINTER;
    }

    g_layout = a_layout;
    g_shiftLayout = a_shiftLayout;
    g_capsOnLayout = a_capsOnLayout;

    return ERROR_SUCCESS;
}

uint8 keyboard_readKey()
{
    while (true) {
        turnOffInts();

        if (g_keyboardBufferPos != 0) {
            break;
        }

        turnOnInts();
        pauseCpu();
    }

    uint8 ret = g_keyboardBuffer[0];

    for (uint32 i = 1; i < g_keyboardBufferPos; i++) {
        g_keyboardBuffer[i - 1] = g_keyboardBuffer[i];
    }

    g_keyboardBufferPos--;

    turnOnInts();

    return ret;
}

void keyboard_intHandler(
    __attribute__((unused)) CpuState *a_state)
{
    helper_keyboardReadScanCode();
}
