#pragma once

#include "Kbd_Config.h"

#include "FreeRTOS.h"
#include "task.h"

#include "Kbd_QueueLinkedList.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern TaskHandle_t Keyboard_task_handle;

    extern Kbd_QueueLinkedListNode *Kbd_RegisterQueue(size_t size);
    extern void Kbd_UnregisterQueue(Kbd_QueueLinkedListNode *node);

    extern bool Kbd_Initialize(void);

#ifdef __cplusplus
}
#endif
