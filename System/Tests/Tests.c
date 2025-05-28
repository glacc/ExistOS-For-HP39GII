#include "Tests.h"

#include <stdio.h>

#include "Test_TestDraw.h"
#include "Test_KbdQueue.h"

void Test_RunTests(void)
{
    printf("- Running Tests -\n\n");

    Test_TestDraw();
    Test_KbdQueue();
}
