#ifndef _PhiOS_drivers_text_mode
#define _PhiOS_drivers_text_mode

#include "include/types.h"
#include "include/errors.h"

#define VGA_WIDTH    80
#define VGA_HEIGHT   25
#define VGA_MEM_ADDR 0xB8000

enum VGA_Colors {
    VGA_ColorBlack            = 0,
    VGA_ColorBlue             = 1,
    VGA_ColorGreen            = 2,
    VGA_ColorCyan             = 3,
    VGA_ColorRed              = 4,
    VGA_ColorMagenta          = 5,
    VGA_ColorBrown            = 6,
    VGA_ColorLightGrey        = 7,
    VGA_ColorDarkGrey         = 8,
    VGA_ColorLightBlue        = 9,
    VGA_ColorLightGreen       = 10,
    VGA_ColorLightCyan        = 11,
    VGA_ColorLightRed         = 12,
    VGA_ColorLightMagenta     = 13,
    VGA_ColorLightBrown       = 14,
    VGA_ColorWhite            = 15
};

void VGA_Init();

void VGA_SetBackgroundColor(
    enum VGA_Colors a_bg
);

void VGA_SetForegroundColor(
    enum VGA_Colors a_fg
);

void VGA_WriteChar(
    char a_c
);

uint32 VGA_WriteString(
    const char *a_str
);

void VGA_WriteBuffer(
    const char *a_buffer, 
    size_t a_len
);

void VGA_WriteColoredChar(
    char a_c, 
    enum VGA_Colors a_bg, 
    enum VGA_Colors a_fg
);

void VGA_WriteColoredString(
    const char *a_str, 
    enum VGA_Colors a_bg,
    enum VGA_Colors 
);

void VGA_WriteColoredBuffer(
    const char *a_buffer, 
    size_t a_len,
    enum VGA_Colors a_bg, 
    enum VGA_Colors a_fg
);

#endif
