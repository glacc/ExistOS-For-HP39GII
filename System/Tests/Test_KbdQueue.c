#include "Test_KbdQueue.h"

#include <stdio.h>
#include <stdint.h>

#include "KbdQueue.h"

const char test_char_list[] = "AbCdEfG HhIiJjKkL";

static void Test_KbdQueue_DispKbdQueueStatus(KbdQueue *kbd_queue)
{
    if (kbd_queue == NULL)
        return;

    printf
    (
        "start_index = %d\nend_index = %d\ncount = %d\n\n",
        kbd_queue->start_index,
        kbd_queue->end_index,
        kbd_queue->count
    );
}

static void Test_KbdQueueWithParameter(size_t size, bool overwrite)
{
    KbdQueue *test_queue = KbdQueue_Create(size, overwrite);
    if (test_queue == NULL)
    {
        printf("Failed to create testing KbdQueue.\n");
        return;
    }

    const char *ptr_test_char = test_char_list;
    while (*ptr_test_char != '\0')
    {
        char current_char = *ptr_test_char;

        KbdQueue_Enqueue(test_queue, (KBDQUEUE_DATA_TYPE)current_char);

        printf("Enqueue: %c, New Count: %d\n", current_char, test_queue->count);

        ptr_test_char++;
    }
    printf("Enqueue Result:\n");
    Test_KbdQueue_DispKbdQueueStatus(test_queue);

    KBDQUEUE_DATA_TYPE output_char;
    while (true)
    {
        if (!KbdQueue_Poll(test_queue, &output_char))
            break;
        
        printf("Dequeue: %c, New Count: %d\n", (char)output_char, test_queue->count);
    }
    printf("Dequeue Result:\n");
    Test_KbdQueue_DispKbdQueueStatus(test_queue);

    KbdQueue_Destroy(test_queue);
}

void Test_KbdQueue(void)
{
    printf("- Test KbdQueue (No Overwrite) -\n");
    Test_KbdQueueWithParameter(32, false);

    printf("- Test KbdQueue (Overwrite Enabled) -\n");
    Test_KbdQueueWithParameter(8, true);
}
