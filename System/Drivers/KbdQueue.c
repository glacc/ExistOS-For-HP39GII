#include "KbdQueue.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <malloc.h>

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
        return NULL;

    kbd_queue->overwrite = overwrite;
    kbd_queue->size = size;

    KbdQueue_Clear(kbd_queue);

    return kbd_queue;
}

void KbdQueue_Destroy(KbdQueue *kbd_queue)
{
    if (kbd_queue == NULL)
        return;

    free(kbd_queue);
}

bool KbdQueue_Enqueue(KbdQueue *kbd_queue, KBDQUEUE_DATA_TYPE key_code)
{
    if (kbd_queue == NULL)
        return false;

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

    return true;
}

bool KbdQueue_Poll(KbdQueue *kbd_queue, KBDQUEUE_DATA_TYPE *output)
{
    if (kbd_queue == NULL)
        return false;

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

    return true;
}
