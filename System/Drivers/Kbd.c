#include "Kbd.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "llapi_code.h"
#include "sys_llapi.h"

#include "FreeRTOS.h"
#include "task.h"

#include "LCD.h"

#include "Kbd_QueueList.h"

TaskHandle_t Keyboard_task_handle;

static Kbd_QueueLinkedList kbd_queue_list;

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

static void Kbd_Task(void *args)
{
    char key_press_bin_buffer[17];
    char key_value_bin_buffer[17];

    while (true)
    {
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

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void Kbd_Initialize(void)
{
    kbd_queue_list.first = NULL;
    kbd_queue_list.last = NULL;

    xTaskCreate
    (
        Kbd_Task,
        "Keyboard",
        1024,
        NULL,
        configMAX_PRIORITIES - 2,
        &Keyboard_task_handle
    );
}

