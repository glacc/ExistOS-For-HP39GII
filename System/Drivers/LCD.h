#pragma once

#include <stdint.h>

#define LCD_SIZE_W      256
#define LCD_SIZE_H      127

#define LCD_INDICATOR_LEFT      (1 << 0)
#define LCD_INDICATOR_RIGHT     (1 << 1)
#define LCD_INDICATOR_A__Z      (1 << 2)
#define LCD_INDICATOR_a__z      (1 << 3)
#define LCD_INDICATOR_BUSY      (1 << 4)
#define LCD_INDICATOR_TX        (1 << 5)
#define LCD_INDICATOR_RX        (1 << 6)

#define LCD_OFFSET_XY(x, y)     ((y * LCD_SIZE_W) + x)

#ifdef __cplusplus
extern "C" {
#endif

    extern void LCD_Update(const uint8_t *buffer);
    extern void LCD_SetIndicators(const uint8_t indicators);

    extern void LCD_PutASCIICharXY(uint8_t *buffer, const char character, int x, int y, int size);
    extern void LCD_PutGBKCharXY(uint8_t *buffer, const uint16_t character, int x, int y);

    extern int LCD_PutCharXY(uint8_t *buffer, const char *ptr_char, int x, int y);

    extern void LCD_PrintStrXY(uint8_t *buffer, const char *str, int x, int y);

#ifdef __cplusplus
}
#endif
