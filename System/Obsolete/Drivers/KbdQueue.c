#include "KbdQueue.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <malloc.h>

#include "FreeRTOS.h"
#include "semphr.h"

static size_t KbdQueue_WrapIndex(size_t size, size_t index)
{
    return index % size;
}

void KbdQueue_Clear(KbdQueue *kbd_queue)
{
    if (kbd_queue == NULL)
        return;

    kbd_queue->count = 0;
    kbd_queue->start_index = 0;
    kbd_queue->end_index = 0;
}

KbdQueue *KbdQueue_Create(size_t size, bool overwrite)
{
    if (size == 0)
        return NULL;
    
    KbdQueue *kbd_queue = (KbdQueue *)malloc(sizeof(KbdQueue) + (size * sizeof(KBDQUEUE_DATA_TYPE)));
    if (kbd_queue == NULL)
        goto Error;

    kbd_queue->wait = xSemaphoreCreateBinary();
    if (kbd_queue->wait == NULL)
        goto Error;
    kbd_queue->mutex = xSemaphoreCreateMutex();
    if (kbd_queue->mutex == NULL)
        goto Error;

    kbd_queue->overwrite = overwrite;
    kbd_queue->size = size;

    KbdQueue_Clear(kbd_queue);

    return kbd_queue;

Error:

    KbdQueue_Destroy(kbd_queue);

    return NULL;
}

void KbdQueue_Destroy(KbdQueue *kbd_queue)
{
    if (kbd_queue == NULL)
        return;

    if (kbd_queue->wait != NULL)
        vSemaphoreDelete(kbd_queue->wait);

    if (kbd_queue->mutex != NULL)
        vSemaphoreDelete(kbd_queue->mutex);

    free(kbd_queue);
}

bool KbdQueue_Enqueue(KbdQueue *kbd_queue, KBDQUEUE_DATA_TYPE key_code)
{
    if (kbd_queue == NULL)
        return false;

    xSemaphoreTake(kbd_queue->mutex, portMAX_DELAY);

    size_t count = kbd_queue->count;
    size_t size = kbd_queue->size;

    if (count == size)
    {
        if (kbd_queue->overwrite)
            kbd_queue->start_index = KbdQueue_WrapIndex(size, kbd_queue->start_index + 1);
        else
            return false;
    }
    else
        kbd_queue->count = count + 1;

    size_t end_index = kbd_queue->end_index;

    if (count == 0)
        kbd_queue->data[end_index] = key_code;
    else
    {
        end_index = KbdQueue_WrapIndex(size, end_index + 1);

        kbd_queue->data[end_index] = key_code;

        kbd_queue->end_index = end_index;
    }

    xSemaphoreGive(kbd_queue->mutex);

    return true;
}

bool KbdQueue_Poll(KbdQueue *kbd_queue, KBDQUEUE_DATA_TYPE *output)
{
    if (kbd_queue == NULL)
        return false;

    xSemaphoreTake(kbd_queue->mutex, portMAX_DELAY);

    size_t count = kbd_queue->count;
    if (count == 0)
        return false;

    size_t start_index = kbd_queue->start_index;

    KBDQUEUE_DATA_TYPE *data = kbd_queue->data;

    if (output != NULL)
        *output = data[start_index];

    if (count > 1)
        kbd_queue->start_index = KbdQueue_WrapIndex(kbd_queue->size, start_index + 1);
    
    kbd_queue->count = count - 1;

    xSemaphoreGive(kbd_queue->mutex);

    return true;
}

bool KbdQueue_WaitForKey(KbdQueue *kbd_queue, TickType_t timeout)
{
    if (kbd_queue == NULL)
        return false;

    if (xSemaphoreTake(kbd_queue->wait, timeout) == pdFALSE)
        return false;

    return true;
}
