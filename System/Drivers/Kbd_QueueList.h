#pragma once

#include "KbdQueue.h"

typedef struct
{
    void *prev;
    void *next;
    KbdQueue *queue;
}
Kbd_QueueLinkedListNode;

typedef struct
{
    Kbd_QueueLinkedListNode *first;
    Kbd_QueueLinkedListNode *last;
}
Kbd_QueueLinkedList;

/*
#ifdef __cplusplus
extern "C" {
#endif

    extern Kbd_QueueList_Initialize(Kbd_QueueLinkedList )

#ifdef __cplusplus
}
#endif
*/
