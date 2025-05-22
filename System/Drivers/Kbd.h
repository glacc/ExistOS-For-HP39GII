#pragma once

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern TaskHandle_t Keyboard_task_handle;

    extern void Kbd_Initialize(void);

#ifdef __cplusplus
}
#endif
