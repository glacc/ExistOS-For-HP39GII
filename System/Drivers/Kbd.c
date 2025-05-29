#include "Kbd.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <malloc.h>

#include "llapi_code.h"
#include "sys_llapi.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "LCD.h"

#include "Kbd_QueueLinkedList.h"

#include "Kbd_TestOutput.h"

static bool kbd_initialized = false;

TaskHandle_t Keyboard_task_handle;

static Kbd_QueueLinkedList kbd_queue_linked_list;

Kbd_QueueLinkedListNode *Kbd_RegisterQueue(size_t size)
{
    return Kbd_QueueLinkedList_CreateNode(&kbd_queue_linked_list, size);
}

void Kbd_UnregisterQueue(Kbd_QueueLinkedListNode *node)
{
    Kbd_QueueLinkedList_DeleteNode(&kbd_queue_linked_list, node);
}

static void Kbd_Task(void *args)
{
    while (true)
    {
        Kbd_CheckAndPrintKeyPress();

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

bool Kbd_Initialize(void)
{
    if (kbd_initialized)
        return true;

    if (!Kbd_QueueLinkedList_InitList(&kbd_queue_linked_list))
        return false;

    BaseType_t task_create_result = xTaskCreate
    (
        Kbd_Task,
        "Keyboard",
        1024,
        NULL,
        configMAX_PRIORITIES - 2,
        &Keyboard_task_handle
    );

    if (task_create_result == pdFALSE)
        return false;

    kbd_initialized = true;

    return true;
}
