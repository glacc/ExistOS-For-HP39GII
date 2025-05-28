#include "Test_TestDraw.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "LCD.h"

static const char title_string[] = "Test Draw";
static const char hello_string[] = "ÄãºÃxwx";
static const char glacc_string[] = "Glacc (aka ¿§à¬½´) qwq";

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

    LCD_PrintStrXY(temp_vram, title_string, 32, 16);
    LCD_PrintStrXY(temp_vram, hello_string, 32, 64);
    LCD_PrintStrXY(temp_vram, glacc_string, 32, 96);

    LCD_Update((const uint8_t *)temp_vram);

    ptr_vram = NULL;

    free((void *)temp_vram);
    temp_vram = NULL;
}
