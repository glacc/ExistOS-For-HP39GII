#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define KBDQUEUE_DATA_TYPE  uint16_t

typedef struct
{
    bool overwrite;
    size_t size;

    size_t count;
    size_t start_index;
    size_t end_index;

    KBDQUEUE_DATA_TYPE data[];
}
KbdQueue;

#ifdef __cplusplus
extern "C" {
#endif

    extern KbdQueue *KbdQueue_Create(size_t size, bool overwrite);
    extern void KbdQueue_Destroy(KbdQueue *kbd_queue);

    extern bool KbdQueue_Enqueue(KbdQueue *kbd_queue, KBDQUEUE_DATA_TYPE key_code);
    extern bool KbdQueue_Poll(KbdQueue *kbd_queue, KBDQUEUE_DATA_TYPE *output);
    extern void KbdQueue_Clear(KbdQueue *kbd_queue);

#ifdef __cplusplus
}
#endif
