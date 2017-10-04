#include "drivers/video/include/vga/text_mode.h"

#include "kernel/include/arch/x86/asm_io.h"

#include "util/kstdlib/include/kstring.h"

#define VGA_MAX_LINES 27

static uint16 g_VGA_buffer[VGA_MAX_LINES * VGA_WIDTH];
static uint32 g_VGA_bufferCurrentRow;
static uint32 g_VGA_bufferCurrentColumn;
static uint32 g_VGA_bufferBaseRow;
static bool   g_VGA_bufferOverflow;

static enum VGA_Colors g_VGA_backgroundColor;
static enum VGA_Colors g_VGA_foregroundColor;

uint32 g_VGA_startRow;
uint32 g_VGA_row;
uint32 g_VGA_column;
uint16 *g_VGA_screen;
bool g_VGA_focusOnLastRow;

static void VGA_DisableCursor()
{
    io_outb(0x3D4, 0x0A);
    io_outb(0x3D5, 0x3f);
}

static void VGA_EnableCursor()
{
    io_outb(0x3D4, 0x0A);
    io_outb(0x3D5, (io_inb(0x3D5) & 0xC0) | 14);

    io_outb(0x3D4, 0x0B);
    io_outb(0x3D5, (io_inb(0x3E0) & 0xE0) | 15);
}

static void VGA_MoveCursor(
    uint16 a_column,
    uint16 a_row)
{
    uint16 position = (a_row * 80) + a_column;
    g_VGA_row = a_row;
    g_VGA_column = a_column;

    io_outb(0x3D4, 0x0F);
    io_outb(0x3D5, (uint8) (position & 0xFF));
    io_outb(0x3D4, 0x0E);
    io_outb(0x3D5, (uint8) ((position >> 8) & 0xFF));
}

static inline uint16 VGA_CreateEntry(
    char a_c,
    enum VGA_Colors a_bg,
    enum VGA_Colors a_fg)
{
    return (uint16) a_c | ((uint16) (a_fg | (a_bg << 4)) << 8);
}

static void VGA_MoveScreen(
    uint32 baseRow)
{
    if (baseRow + VGA_HEIGHT < VGA_MAX_LINES) {
        uint16 *src = &g_VGA_buffer[baseRow * VGA_WIDTH];
        kmemcpy(g_VGA_screen, src, VGA_WIDTH * VGA_HEIGHT * 2);
    } else {
        uint16 *src = &g_VGA_buffer[baseRow * VGA_WIDTH];
        uint32 lines = VGA_MAX_LINES - baseRow;
        kmemcpy(g_VGA_screen, src, lines * VGA_WIDTH * 2);
        kmemcpy(g_VGA_screen + (lines * VGA_WIDTH), g_VGA_buffer, (VGA_HEIGHT - lines) * VGA_WIDTH * 2);
    }
}

static void VGA_NewLine()
{
    g_VGA_bufferCurrentColumn = 0;
    g_VGA_bufferCurrentRow++;

    if (g_VGA_bufferCurrentRow == VGA_MAX_LINES) {
        g_VGA_bufferOverflow = true;
        g_VGA_bufferCurrentRow = 0;
    }

    if (g_VGA_bufferOverflow == true) {
        bool moveScreen = g_VGA_bufferBaseRow == g_VGA_startRow ? true : false;

        g_VGA_bufferBaseRow = g_VGA_bufferCurrentRow + 1;
        g_VGA_bufferBaseRow %= VGA_MAX_LINES;

        if (moveScreen) {
            VGA_MoveScreen(g_VGA_bufferBaseRow);
            g_VGA_startRow++;
            g_VGA_startRow %= VGA_MAX_LINES;
        }
    }

    uint32 index = g_VGA_bufferCurrentRow * VGA_WIDTH;
    uint16 ch = VGA_CreateEntry(' ', g_VGA_backgroundColor, g_VGA_foregroundColor);
    for (uint32 i = 0; i < VGA_WIDTH; i++) {
        g_VGA_buffer[index + i] = ch;
    }
}

static void VGA_ScreenNewLine()
{
    g_VGA_column = 0;
    g_VGA_row++;

    g_VGA_startRow++;
    g_VGA_startRow %= VGA_MAX_LINES;

    if (g_VGA_row == VGA_HEIGHT) {
        for (uint32 i = 1; i < VGA_HEIGHT; i++) {
            uint32 index1 = i * VGA_WIDTH;
            uint32 index2 = (i - 1) * VGA_WIDTH;
            for (uint32 j = 0; j < VGA_WIDTH; j++) {
                g_VGA_screen[index2 + j] = g_VGA_screen[index1 + j];
            }
        }

        g_VGA_row--;

        uint16 ch = VGA_CreateEntry(' ', g_VGA_backgroundColor, g_VGA_foregroundColor);
        for (uint32 i = 0; i < VGA_WIDTH; i++) {
            g_VGA_screen[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = ch;
        }
    }
}

void VGA_Init()
{
    g_VGA_startRow = 0;
    g_VGA_row = 0;
    g_VGA_column = 0;
    g_VGA_screen = (uint16*) VGA_MEM_ADDR;
    g_VGA_focusOnLastRow = true;

    g_VGA_bufferCurrentColumn = 0;
    g_VGA_bufferCurrentRow = 0;
    g_VGA_bufferBaseRow = 0;
    g_VGA_bufferOverflow = false;

    g_VGA_foregroundColor = VGA_ColorLightGrey;
    g_VGA_backgroundColor = VGA_ColorBlack;

    uint16 ch = VGA_CreateEntry(' ', g_VGA_backgroundColor, g_VGA_foregroundColor);
    uint32 index = 0;

    for (uint32 y = 0; y < VGA_HEIGHT; y++) {
        index = y * VGA_WIDTH;
        for (uint32 x = 0; x < VGA_WIDTH; x++) {
            g_VGA_screen[index + x] = ch;
        }
    }

    for (uint32 y = 0; y < VGA_MAX_LINES; y++) {
        index = y * VGA_WIDTH;
        for (uint32 x = 0; x < VGA_WIDTH; x++) {
            g_VGA_buffer[index + x] = ch;
        }
    }

    VGA_MoveCursor(0, 0);
    VGA_DisableCursor();
    VGA_EnableCursor();
}

void VGA_SetBackgroundColor(
    enum VGA_Colors a_bg)
{
    g_VGA_backgroundColor = a_bg;
}

void VGA_SetForegroundColor(
    enum VGA_Colors a_fg)
{
    g_VGA_foregroundColor = a_fg;
}

void VGA_WriteChar(
    char a_c)
{
    VGA_WriteColoredChar(a_c, g_VGA_backgroundColor, g_VGA_foregroundColor);
}

uint32 VGA_WriteString(
    const char *a_str)
{
    VGA_WriteColoredString(a_str, g_VGA_backgroundColor, g_VGA_foregroundColor);
    return ERROR_SUCCESS;
}

void VGA_WriteBuffer(
    const char *a_buffer,
    size_t a_len)
{
    VGA_WriteColoredBuffer(a_buffer, a_len, g_VGA_backgroundColor, g_VGA_foregroundColor);
}

void VGA_WriteColoredChar(
    char a_c,
    enum VGA_Colors a_bg,
    enum VGA_Colors a_fg)
{
    size_t index;
    uint16 entry;

    switch (a_c) {
        case '\n':
            VGA_NewLine();

            if (g_VGA_focusOnLastRow == true) {
                VGA_ScreenNewLine();
            }

            break;

        case '\r':
            g_VGA_column = 0;
            break;

        case '\b':
            // TODO: implement this
            break;

        case '\t':
            // TODO: implement this
            break;

        default:
            entry = VGA_CreateEntry(a_c, a_bg, a_fg);

            index = g_VGA_bufferCurrentRow * VGA_WIDTH + g_VGA_bufferCurrentColumn;
            g_VGA_buffer[index] = entry;

            g_VGA_bufferCurrentColumn++;
            if (g_VGA_bufferCurrentColumn == VGA_WIDTH) {
                VGA_NewLine();
            }

            if (g_VGA_focusOnLastRow == true) {
                g_VGA_screen[g_VGA_row * VGA_WIDTH + g_VGA_column] = entry;
                g_VGA_column++;
                if (g_VGA_column == VGA_WIDTH) {
                    VGA_ScreenNewLine();
                }
            }

            break;
    }

    VGA_MoveCursor(g_VGA_column, g_VGA_row);
}

void VGA_WriteColoredString(
    const char *a_str,
    enum VGA_Colors a_bg,
    enum VGA_Colors a_fg)
{
    for (size_t i = 0; a_str[i] != '\0'; i++) {
        VGA_WriteColoredChar(a_str[i], a_bg, a_fg);
    }
}

void VGA_WriteColoredBuffer(
    const char *a_buffer,
    size_t a_len,
    enum VGA_Colors a_bg,
    enum VGA_Colors a_fg)
{
    for (size_t i = 0; i < a_len; i++) {
        VGA_WriteColoredChar(a_buffer[i], a_bg, a_fg);
    }
}
