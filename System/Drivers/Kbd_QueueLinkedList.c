#include "Kbd_QueueLinkedList.h"

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

Kbd_QueueLinkedListNode *Kbd_QueueLinkedList_CreateNode(Kbd_QueueLinkedList *kbd_queue_linked_list, size_t size)
{
    if (kbd_queue_linked_list == NULL)
        return NULL;

    // Node Creation //

    if (size == 0)
        return NULL;

    Kbd_QueueLinkedListNode *new_node = (Kbd_QueueLinkedListNode *)malloc(sizeof(Kbd_QueueLinkedListNode));
    if (new_node == NULL)
        return NULL;

    QueueHandle_t key_queue_handle = xQueueCreate(size, sizeof(KEYCODE_DATA_TYPE));
    if (key_queue_handle == NULL)
    {
        free((void *)new_node);
        return NULL;
    }
    new_node->key_queue_handle = key_queue_handle;

    // Queue Linked List Modification //

    xSemaphoreTake(kbd_queue_linked_list->mutex, portMAX_DELAY);

    new_node->prev = (Kbd_QueueLinkedListNode*)kbd_queue_linked_list->last;
    if (kbd_queue_linked_list->last != NULL)
        ((Kbd_QueueLinkedListNode*)kbd_queue_linked_list->last)->next = new_node;

    if (kbd_queue_linked_list->first == NULL)
        kbd_queue_linked_list->first = new_node;
    kbd_queue_linked_list->last = new_node;

    new_node->next = NULL;

    xSemaphoreGive(kbd_queue_linked_list->mutex);

    return new_node;
}

void Kbd_QueueLinkedList_DeleteNode(Kbd_QueueLinkedList *kbd_queue_linked_list, Kbd_QueueLinkedListNode *node)
{
    if (kbd_queue_linked_list == NULL)
        return;

    // Get Prev/Next Nodes //

    if (node == NULL)
        return;

    xSemaphoreTake(kbd_queue_linked_list->mutex, portMAX_DELAY);

    Kbd_QueueLinkedListNode *prev = (Kbd_QueueLinkedListNode *)node->prev;
    Kbd_QueueLinkedListNode *next = (Kbd_QueueLinkedListNode *)node->next;

    // Neighbor Nodes //

    if (prev != NULL)
        prev->next = next;
    if (next != NULL)
        next->prev = prev;

    // First / Last Node //

    if (kbd_queue_linked_list->first == node)
        kbd_queue_linked_list->first = next;
    if (kbd_queue_linked_list->last == node)
        kbd_queue_linked_list->last = prev;

    // Delete Node //

    QueueHandle_t key_queue_handle = node->key_queue_handle;
    if (key_queue_handle != NULL)
        vQueueDelete(key_queue_handle);

    xSemaphoreGive(kbd_queue_linked_list->mutex);

    free((void *)node);
}

bool Kbd_QueueLinkedList_InitList(Kbd_QueueLinkedList *kbd_queue_linked_list)
{
    if (kbd_queue_linked_list == NULL)
        return false;

    kbd_queue_linked_list->first = NULL;
    kbd_queue_linked_list->last = NULL;

    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    if (mutex == NULL)
        return false;
    kbd_queue_linked_list->mutex = mutex;

    return true;
}

void Kbd_QueueLinkedList_DeinitList(Kbd_QueueLinkedList *kbd_queue_linked_list)
{
    if (kbd_queue_linked_list == NULL)
        return;

    SemaphoreHandle_t mutex = kbd_queue_linked_list->mutex;
    if (mutex != NULL)
        vSemaphoreDelete(mutex);

    kbd_queue_linked_list->mutex = NULL;
}
