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

void LCD_PutASCIICharXY(const uint8_t *buffer, const char character, int x, int y)
{

}

void LCD_PutGBKCharXY(const uint8_t *buffer, const uint16_t character, int x, int y)
{
    extern uint32_t fonts_hzk_start;
    extern uint32_t fonts_hzk_end;

    int lv = (character & 0xFF) - 0xa1;
    int hv = (character >> 8) - 0xa1;
    uint32_t offset = (uint32_t)(94 * hv + lv) * 32;
    uint8_t *font_data = (uint8_t *)(((uint32_t)&fonts_hzk_start) + offset);
    if ((uint32_t)font_data > (uint32_t)&fonts_hzk_end) {
        return;
    }

    const int buffer_stride = LCD_SIZE_W - 16;
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

void LCD_PutCharXY(const uint8_t *buffer, const char *ptr_char, int x, int y)
{

}
