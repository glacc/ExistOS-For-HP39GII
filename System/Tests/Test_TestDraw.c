#include "Test_TestDraw.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "LCD.h"

static const char test_draw_string[] = "Test Draw";
static const char gbk_hello_string[] = "ÄãºÃ";
static const char hello_world_string[] = "Hello, world!";
static const char glacc_string[] = "Glacc qwq";

void Test_TestDraw(void)
{
    printf("- Runing Test_TestDraw -\n\n");

    uint8_t *temp_vram = (uint8_t *)malloc(LCD_SIZE_W * LCD_SIZE_H);
    if (temp_vram == NULL)
        return;

    uint8_t *ptr_vram = temp_vram;

    for (int y = 0; y < LCD_SIZE_H; y++)
    {
        for (int x = 0; x < LCD_SIZE_W; x++)
            *ptr_vram++ = LCD_SIZE_W - (uint8_t)x;
    }

    LCD_PrintStrXY(temp_vram, test_draw_string, 4, 4, LCD_FONT_SIZE_16, 0);
    LCD_PrintStrXY(temp_vram, gbk_hello_string, 4, 24, LCD_FONT_SIZE_16, 0);

    const int grayscale_offset_y = 56;
    LCD_PrintStrXY(temp_vram, hello_world_string, 4, grayscale_offset_y, LCD_FONT_SIZE_8, 127);
    LCD_PrintStrXY(temp_vram, hello_world_string, 4, grayscale_offset_y + 12, LCD_FONT_SIZE_12, 63);
    LCD_PrintStrXY(temp_vram, hello_world_string, 4, grayscale_offset_y + 12 + 16, LCD_FONT_SIZE_16, 0);

    LCD_PrintStrXY(temp_vram, glacc_string, 32, 108, LCD_FONT_SIZE_12, 0);

    LCD_Update((const uint8_t *)temp_vram);

    ptr_vram = NULL;

    free((void *)temp_vram);
    temp_vram = NULL;
}
