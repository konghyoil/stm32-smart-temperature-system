#ifndef TEMP_FREERTOS_H
#define TEMP_FREERTOS_H

#include "FreeRTOS.h"
#include "main.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"

#include "Gpio.h"
#include "Fnd.h"
#include "Temperature_Sensor.h"
#include "State.h"
#include "Oled.h"
#include "Uart.h"

extern SemaphoreHandle_t ButtonSemaphore;
extern SemaphoreHandle_t State_Mutex;
extern SemaphoreHandle_t Uart_Mutex;

void Semaphore_Init();
void Mutex_Init();
void FreeRtos_Init();

void Led_Task(void * argument);
void Fnd_Task(void *argument);
void Temp_Task(void *argument);
void State_Task(void *argument);
void Uart_Task(void *argument);
void Oled_Task(void *argument);

#endif
