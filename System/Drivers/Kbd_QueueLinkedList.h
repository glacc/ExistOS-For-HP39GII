#pragma once

#include <stdbool.h>

#include "Kbd_Config.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

typedef struct
{
    void *prev;
    void *next;
    QueueHandle_t key_queue_handle;
}
Kbd_QueueLinkedListNode;

typedef struct
{
    Kbd_QueueLinkedListNode *first;
    Kbd_QueueLinkedListNode *last;

    SemaphoreHandle_t mutex;
}
Kbd_QueueLinkedList;

#ifdef __cplusplus
extern "C" {
#endif

    extern Kbd_QueueLinkedListNode *Kbd_QueueLinkedList_CreateNode(Kbd_QueueLinkedList *kbd_queue_linked_list, size_t size);
    extern void Kbd_QueueLinkedList_DeleteNode(Kbd_QueueLinkedList *kbd_queue_linked_list, Kbd_QueueLinkedListNode *node);

    extern bool Kbd_QueueLinkedList_InitList(Kbd_QueueLinkedList *kbd_queue_linked_list);
    extern void Kbd_QueueLinkedList_DeinitList(Kbd_QueueLinkedList *kbd_queue_linked_list);

#ifdef __cplusplus
}
#endif
