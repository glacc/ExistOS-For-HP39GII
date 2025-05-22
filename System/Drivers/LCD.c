#include "LCD.h"

#include <stddef.h>
#include <stdint.h>

#include "llapi_code.h"
#include "sys_llapi.h"

void LCD_Update(const uint8_t *buffer)
{
    if (buffer == NULL)
        return;

    ll_disp_put_area((uint8_t *)buffer, 0, 0, LCD_SIZE_W - 1, LCD_SIZE_H - 1);
}

void LCD_SetIndicators(const uint8_t indicators)
{
    ll_disp_set_indicator(indicators, -1);
}

void LCD_PutASCIICharXY(uint8_t *buffer, const char character, int x, int y, int size)
{
    if (buffer == NULL)
        return;

    extern const unsigned char VGA_Ascii_5x8[];
    extern const unsigned char VGA_Ascii_6x12[];
    extern const unsigned char VGA_Ascii_8x16[];

    if (character < ' ' || character > '~' + 1)
        return;

    const int font_width = 8;
    int font_height;

    uint8_t *ptr_font;

    switch (size)
    {
        case 8:
            font_height = 8;
            ptr_font = (uint8_t *)VGA_Ascii_5x8;
            break;
        case 12:
            font_height = 12;
            ptr_font = (uint8_t *)VGA_Ascii_6x12;
            break;
        case 16:
            font_height = 16;
            ptr_font = (uint8_t *)VGA_Ascii_8x16;
            break;
    }

    size_t ptr_offset = (character - ' ') * font_height;
    ptr_font += ptr_offset;

    size_t buffer_stride = LCD_SIZE_W - font_width;
    uint8_t *ptr_buffer = buffer + LCD_OFFSET_XY(x, y);

    for (int font_y = 0; font_y < font_height; font_y++)
    {
        uint8_t font_pixels = *ptr_font;

        for (int font_x = 0; font_x < font_width; font_x++)
        {
            if ((font_pixels & 0x80) != 0)
                *ptr_buffer = 0;
            ptr_buffer++;

            font_pixels <<= 1;
        }

        ptr_font++;

        ptr_buffer += buffer_stride;
    }
}

void LCD_PutGBKCharXY(uint8_t *buffer, const uint16_t character, int x, int y)
{
    if (buffer == NULL)
        return;

    extern uint32_t fonts_hzk_start;
    extern uint32_t fonts_hzk_end;

    int lv = (character & 0xFF) - 0xa1;
    int hv = (character >> 8) - 0xa1;
    uint32_t offset = (uint32_t)(94 * hv + lv) * 32;
    uint8_t *font_data = (uint8_t *)(((uint32_t)&fonts_hzk_start) + offset);
    if ((uint32_t)font_data > (uint32_t)&fonts_hzk_end) {
        return;
    }

    const size_t buffer_stride = LCD_SIZE_W - 16;
    uint8_t *ptr_buffer = buffer + LCD_OFFSET_XY(x, y);

    int py = y;
    for (int i = 0; i < 32; i += 2)
    {
        int px = x;
        uint8_t font_pixels;

        font_pixels = font_data[i];
        for (int t = 0; t < 8; t++)
        {
            if (font_pixels & 0x80)
                *ptr_buffer = 0;
            ptr_buffer++;

            px++;

            font_pixels <<= 1;
        }

        font_pixels = font_data[i + 1];
        for (int t = 0; t < 8; t++)
        {
            if (font_pixels & 0x80)
                *ptr_buffer = 0;
            ptr_buffer++;
            
            px++;
            
            font_pixels <<= 1;
        }

        ptr_buffer += buffer_stride;

        py++;
    }
}

int LCD_PutCharXY(uint8_t *buffer, const char *ptr_char, int x, int y)
{
    if (buffer == NULL || ptr_char == NULL)
        return 8;

    char current_char = *ptr_char;

    if ((uint8_t)current_char < 0x80)
    {
        // ASCII Character //

        LCD_PutASCIICharXY(buffer, current_char, x, y, 16);

        return 8;
    }
    else
    {
        // GBK Character //

        uint16_t gbk_code = ((uint16_t)*(ptr_char + 1)) | ((uint16_t)current_char << 8);

        LCD_PutGBKCharXY(buffer, gbk_code, x, y);

        return 16;
    }
}

void LCD_PrintStrXY(uint8_t *buffer, const char *str, int x, int y)
{
    if (buffer == NULL || str == NULL)
        return;

    int px = x;
    int py = y;

    const char *ptr_str = str;
    while (*ptr_str != '\0')
    {
        int x_increment = LCD_PutCharXY(buffer, ptr_str, px, py);

        px += x_increment;

        if (*ptr_str >= 0x80)
            ptr_str++;

        ptr_str++;
    }
}
