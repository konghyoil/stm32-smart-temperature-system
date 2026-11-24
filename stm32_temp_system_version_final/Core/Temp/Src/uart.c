#include "uart.h"
#include "TempRtos.h"

uint8_t rx_dma_buf[UART_RX_BUF_SIZE];
uint8_t rx_buf[UART_RX_BUF_SIZE];
volatile uint16_t rx_len = 0;

void transmit_data(SystemData_t* data)
{
	char buf[20];
	static SystemData_t prev = {0};
	static uint8_t prev_flag=0;

	if(data->state != prev.state)
	{
		if(data->state == STATE_OFF)
			strcpy(buf, "STATE OFF\n");

		else if(data->state == STATE_IDLE)
			strcpy(buf, "STATE IDLE\n");

		else if(data->state == STATE_HEATING)
			strcpy(buf, "STATE HEATING\n");

		else
			strcpy(buf, "STATE ERROR\n");

		HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
	}

	if(flag != prev_flag)
	{
		if(flag)
		{
			strcpy(buf, "POWER ON\n");
		}
		else
		{
			strcpy(buf, "POWER OFF\n");
		}

		HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
	}

	if(data->temp_current != prev.temp_current)
	{
		sprintf(buf, "TEMPERATURE %d.%d\n", (int)data->temp_current, (int)(data->temp_current * 10) % 10);

		HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
	}

	prev_flag = flag;
	prev = *data;
}

void UART_RX_Start()
{
    memset(rx_dma_buf, 0, sizeof(rx_dma_buf));
    memset(rx_buf, 0, sizeof(rx_buf));

    rx_len = 0;

    HAL_UARTEx_ReceiveToIdle_DMA(
        &huart1,
        rx_dma_buf,
        UART_RX_BUF_SIZE
    );

    /* DMA Half Transfer 인터럽트 비활성화 */
    if (huart1.hdmarx != NULL)
    {
        __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
    }
}

float UART_RX_ParseTemp(char *rx_buf)
{
    char *endptr;
    float temp;

    if (strncmp(rx_buf, "TEMP ", 5) != 0)
        return -1.0f;

    temp = strtof(rx_buf + 5, &endptr);

    /* 숫자가 없는 경우 */
    if (endptr == rx_buf + 5)
        return -1.0f;

    /* 줄바꿈 문자 처리 */
    if (*endptr == '\r')
        endptr++;

    if (*endptr == '\n')
        endptr++;

    if (*endptr != '\0')
        return -1.0f;

    /* 허용 온도 범위 */
    if (temp < 0.0f || temp > 100.0f)
        return -1.0f;

    return temp;
}

void UART_RX_ProcessPower(char *rx_buf)
{
    rx_buf[strcspn(rx_buf, "\r\n")] = '\0';

    if (strcmp(rx_buf, "POWER ON") == 0)
    {
        flag = 1;
    }
    else if (strcmp(rx_buf, "POWER OFF") == 0)
    {
        flag = 0;
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        if (Size < UART_RX_BUF_SIZE)
        {
            memcpy(rx_buf, rx_dma_buf, Size);
            rx_buf[Size] = '\0';
            rx_len = Size;

            xSemaphoreGiveFromISR(UartRxsemaphore, NULL);
        }
    }

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_dma_buf, sizeof(rx_dma_buf));

    if (huart1.hdmarx != NULL)
    {
    	__HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
    }
}
