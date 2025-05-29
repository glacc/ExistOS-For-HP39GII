#include "Test_KbdQueue.h"

#include <stdio.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "Kbd_QueueLinkedList.h"

static const char test_char_list[] = "AbCdEfG HhIiJjKkL";

static void Test_KbdQueue_LinkedListNode(Kbd_QueueLinkedListNode *node)
{
    if (node == NULL)
        return;

    printf("- Test Kbd_QueueLinkedListNode (%p) -\n\n", (void *)node);

    QueueHandle_t key_queue_handle = node->key_queue_handle;

    const char *ptr_test_char = test_char_list;
    while (true)
    {
        char current_char = *ptr_test_char;
        if (current_char == '\0')
            break;

        KEYCODE_DATA_TYPE char_in_datatype = (KEYCODE_DATA_TYPE)current_char;
        if (xQueueSend(key_queue_handle, &char_in_datatype, 0) == errQUEUE_FULL)
        {
            printf("Queue Full.\n");
            break;
        }

        printf("Enqueue: %c\n", current_char);

        ptr_test_char++;
    }

    putchar('\n');

    while (true)
    {
        /*
        UBaseType_t queue_messages_waiting = uxQueueMessagesWaiting(key_queue_handle);
        printf("(%u) ", queue_messages_waiting);
        if (queue_messages_waiting == 0)
        {
            printf("Queue Empty Now.\n");
            break;
        }
        */

        KEYCODE_DATA_TYPE char_received_in_datatype = 0;
        if (xQueueReceive(key_queue_handle, &char_received_in_datatype, 0) == errQUEUE_EMPTY)
        {
            printf("Queue Empty Now.\n");
            break;
        }

        char char_received = (char)char_received_in_datatype;

        printf("Dequeue: %c\n", char_received);
    }

    putchar('\n');
}

static void Test_KbdQueue_ListLinkedListNodes(Kbd_QueueLinkedList *kbd_queue_linked_list)
{
    if (kbd_queue_linked_list == NULL)
        return;

    printf("Kbd_QueueLinkedList at %p\n", (void *)kbd_queue_linked_list);
    printf
    (
        "F: %-12pL: %-12p\n",
        (void *)kbd_queue_linked_list->first,
        (void *)kbd_queue_linked_list->last
    );

    int index = -1;
    Kbd_QueueLinkedListNode *curr_node = kbd_queue_linked_list->first;
    while (curr_node != NULL)
    {
        index++;

        printf
        (
            "%d\tN: %-12pQ: %-12pP: %-12pN: %-12p\n",
            index,
            (void *)curr_node, (void *)curr_node->key_queue_handle,
            (void *)curr_node->prev, (void *)curr_node->next
        );

        curr_node = (Kbd_QueueLinkedListNode *)curr_node->next;
    }

    if (index == -1)
        printf("Empty.\n");

    putchar('\n');
}

void Test_KbdQueue(void)
{
    printf("- Runing Test_KbdQueue -\n\n");

    Kbd_QueueLinkedList key_queue_linked_list;
    Kbd_QueueLinkedList_InitList(&key_queue_linked_list);
    Test_KbdQueue_ListLinkedListNodes(&key_queue_linked_list);

    Kbd_QueueLinkedListNode *node_1 = Kbd_QueueLinkedList_CreateNode(&key_queue_linked_list, 32);
    Kbd_QueueLinkedListNode *node_2 = Kbd_QueueLinkedList_CreateNode(&key_queue_linked_list, 8);
    Test_KbdQueue_ListLinkedListNodes(&key_queue_linked_list);

    Test_KbdQueue_LinkedListNode(node_1);
    Test_KbdQueue_LinkedListNode(node_2);

    Kbd_QueueLinkedList_DeleteNode(&key_queue_linked_list, node_1);
    Test_KbdQueue_ListLinkedListNodes(&key_queue_linked_list);
    Kbd_QueueLinkedList_DeleteNode(&key_queue_linked_list, node_2);
    Test_KbdQueue_ListLinkedListNodes(&key_queue_linked_list);

    Kbd_QueueLinkedList_DeinitList(&key_queue_linked_list);
}
