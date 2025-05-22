#include "Test_TestDraw.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "LCD.h"

#include "SystemUI.h"

void Test_TestDraw(void)
{
    printf("Running Test_TestDraw\n");

    uint8_t *temp_vram = (uint8_t *)malloc(LCD_PIX_W * LCD_PIX_H);
    if (temp_vram == NULL)
        return;

    LCD_PutGBKCharXY(temp_vram, '²â', 16, 16);

    uint8_t *ptr_vram = temp_vram;

    for (int y = 0; y < LCD_PIX_H; y++)
    {
        for (int x = 0; x < LCD_PIX_W; x++)
            *ptr_vram++ = (uint8_t)x;
    }

    LCD_Update((const uint8_t *)temp_vram);

    ptr_vram = NULL;

    free((void *)temp_vram);
    temp_vram = NULL;
}
