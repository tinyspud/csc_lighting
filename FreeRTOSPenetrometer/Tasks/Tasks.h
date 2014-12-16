#ifndef __TASKS_H_
#define __TASKS_H_

#include <FreeRTOS.h>
#include "os_task.h"

/* Include Task headers */
#include "GPSTask.h"
#include "DisplayTask.h"
#include "LogTask.h"
#include "UARTMuxTask.h"
#include "Semaphores.h"
#include "UITask.h"

/* Create handles */
TaskHandle_t xDisplayTaskHandle;
TaskHandle_t xGPSTaskHandle;
TaskHandle_t xLogTaskHandle;
TaskHandle_t xUARTMuxTaskHandle;
TaskHandle_t xUITaskHandle;




#endif

