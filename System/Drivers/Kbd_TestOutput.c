#include "Kbd_TestOutput.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "llapi_code.h"
#include "sys_llapi.h"

static void Kbd_UInt32ToBinary(uint32_t num, char *str_buffer)
{
    if (str_buffer == NULL)
        return;

    for (int i = 0; i < 32; i++)
    {
        char curr_char;
        
        if ((num & 0x80000000) != 0)
            curr_char = '1';
        else
            curr_char = '0';

        *str_buffer++ = curr_char;

        num <<= 1;
    }

    *str_buffer = '\0';
}

static void Kbd_UInt16ToBinary(uint16_t num, char *str_buffer)
{
    if (str_buffer == NULL)
        return;

    for (int i = 0; i < 16; i++)
    {
        char curr_char;
        
        if ((num & 0x8000) != 0)
            curr_char = '1';
        else
            curr_char = '0';

        *str_buffer++ = curr_char;

        num <<= 1;
    }

    *str_buffer = '\0';
}

void Kbd_CheckAndPrintKeyPress(void)
{
    char key_press_bin_buffer[17];
    char key_value_bin_buffer[17];

    uint32_t key = ll_vm_check_key();

    uint16_t key_press = key >> 16;
    uint16_t key_value = key & 0xFFFF;

    if (key_press != 0)
    {
        Kbd_UInt16ToBinary(key_press, key_press_bin_buffer);
        Kbd_UInt16ToBinary(key_value, key_value_bin_buffer);

        printf("Key pressed:\n");
        printf("key_press = %s\nkey_value = %s\n", key_press_bin_buffer, key_value_bin_buffer);
    }
}
