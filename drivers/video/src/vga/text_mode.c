#include "drivers/video/include/vga/text_mode.h"

#include "kernel/include/arch/x86/asm_io.h"

uint32 g_VGA_row;
uint32 g_VGA_column;
enum VGA_Colors g_VGA_backgroundColor;
enum VGA_Colors g_VGA_foregroundColor;
uint16 *g_VGA_buffer;

void VGA_Init()
{
    g_VGA_row = 0;
    g_VGA_column = 0;

    g_VGA_foregroundColor = VGA_ColorLightGrey;
    g_VGA_backgroundColor = VGA_ColorBlack;

    g_VGA_buffer = (uint16*) VGA_MEM_ADDR;

    VGA_Clear();
}

void VGA_MoveCursor(
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

uint16 VGA_CreateEntry(
    char a_c, enum VGA_Colors
    a_bg, enum VGA_Colors a_fg)
{
    return (uint16) a_c | ((uint16) (a_fg | (a_bg << 4)) << 8);
}

void VGA_WriteChar(
    char a_c)
{
    VGA_WriteColoredChar(a_c, g_VGA_backgroundColor, g_VGA_foregroundColor);
}

void VGA_WriteString(
    const char *a_str)
{
    VGA_WriteColoredString(a_str, g_VGA_backgroundColor, g_VGA_foregroundColor);
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
            g_VGA_column = 0;
            g_VGA_row++;

            if (g_VGA_row == VGA_HEIGHT) {
                VGA_Scroll();
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

            index = g_VGA_row * VGA_WIDTH + g_VGA_column;
            g_VGA_buffer[index] = entry;

            if (++g_VGA_column == VGA_WIDTH) {
                g_VGA_column = 0;
                g_VGA_row++;

                if (g_VGA_row == VGA_HEIGHT) {
                    VGA_Scroll();
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

void VGA_Scroll()
{
    for (uint32 i = 0; i < VGA_HEIGHT - 1; i++)  {
        for (uint32 j = 0; j < VGA_WIDTH; j++) {
            g_VGA_buffer[i * VGA_WIDTH + j] = g_VGA_buffer[(i + 1) * VGA_WIDTH + j];
        }
    }

    uint16 blank = VGA_CreateEntry(' ', g_VGA_backgroundColor, g_VGA_foregroundColor);
    for (uint32 i = 0; i < VGA_WIDTH; i++) {
        g_VGA_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = blank;
    }

    g_VGA_row = VGA_HEIGHT - 1;
}

void VGA_Clear()
{
    for (uint32 y = 0; y < VGA_HEIGHT; y++) {
        for (uint32 x = 0; x < VGA_WIDTH; x++) {
            uint32 index = y * VGA_WIDTH + x;
            g_VGA_buffer[index] = VGA_CreateEntry(' ', g_VGA_backgroundColor, g_VGA_foregroundColor);
        }
    }

    g_VGA_row = 0;
    g_VGA_column = 0;
}
