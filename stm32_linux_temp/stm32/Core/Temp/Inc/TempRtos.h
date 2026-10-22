#ifndef TEMPRTOS_H
#define TEMPRTOS_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dht11_driver.h"
//#include "ssd1306.h"
//#include "fonts.h"
#include "display.h"
#include "Shared_Resource.h"
#include "control.h"
#include "uart.h"

extern SemaphoreHandle_t UartRxsemaphore;

void Task_Init(void);
void Display_Task(void *argument);
void Sensor_Task(void* argument);
void Control_Task(void *argument);
void UartTx_Task(void* argument);
void UartRx_Task(void* argument);

void Data_read(SystemData_t* data);
void Data_write(SystemData_t data);

#endif
