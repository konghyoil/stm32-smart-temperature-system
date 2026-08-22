#include "Uart.h"

HAL_StatusTypeDef Uart_print(char *tx, int size)
{
	return HAL_UART_Transmit(&huart1, (uint8_t *)tx, size, 100);
}

HAL_StatusTypeDef Uart_work(unsigned int state, int temp_target, float temp_current)
{
	static char uart_buffer[100];
	static char ch_state[10] = {0};
	static char prev_buffer[100] = {0};

	int ret;

	if(state == OFF_STATE)
	{
		strcpy(ch_state, "OFF");
	}
	else if(state == IDLE_STATE)
	{
		strcpy(ch_state, "IDLE");
	}
	else if(state == HEATING_STATE)
	{
		strcpy(ch_state, "HEATING");
	}
	else
	{
		strcpy(ch_state, "ERROR");
	}

	snprintf(uart_buffer, sizeof(uart_buffer),"state : %s\r\ntemp_target : %d, temp_current : %d\r\n\r\n", ch_state, temp_target, (int)temp_current);

	if(strcmp(uart_buffer, prev_buffer) != 0)
	{
		ret = Uart_print(uart_buffer, strlen(uart_buffer));
		if(ret != HAL_OK) return ret;

		strcpy(prev_buffer, uart_buffer);
	}

	return ret;
}
