#ifndef UART_H
#define UART_H

#include "main.h"
#include "Shared_Resource.h"
#include "control.h"
#include "semphr.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define UART_RX_BUF_SIZE 64

extern uint8_t rx_buf[UART_RX_BUF_SIZE];
extern volatile uint16_t rx_len;

void transmit_data(SystemData_t* data);
void UART_RX_Start();
float UART_RX_ParseTemp(char *rx_buf);
void UART_RX_ProcessPower(char *rx_buf);

#endif
