#ifndef UART_H
#define UART_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "FreeRTOS.h"
#include "main.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"

#include "Temp_FreeRtos.h"
#include "State.h"

HAL_StatusTypeDef Uart_print(char *tx, int size);
HAL_StatusTypeDef Uart_work(unsigned int state, int temp_target, float temp_current);

#endif
