#include "Gpio.h"

volatile unsigned int button_time = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == Start_Button_Pin)
	{
		if(xTaskGetTickCountFromISR() - button_time >= 300)
		{
			button_time = xTaskGetTickCountFromISR();

			xSemaphoreGiveFromISR(ButtonSemaphore, NULL);
		}
	}
}

int Led_onoff(int button_flag)
{
	if(xSemaphoreTake(ButtonSemaphore, portMAX_DELAY) == pdPASS)
	{
		button_flag = !button_flag;
	}

	if(button_flag)
	{
		HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_RESET);
	}

	xSemaphoreTake(State_Mutex, portMAX_DELAY);
	System_Data.button_flag = button_flag;
	xSemaphoreGive(State_Mutex);

	return button_flag;
}

void Relay_on()
{
	HAL_GPIO_WritePin(Relay_Ctrl_GPIO_Port, Relay_Ctrl_Pin, GPIO_PIN_SET);
}

void Relay_off()
{
	HAL_GPIO_WritePin(Relay_Ctrl_GPIO_Port, Relay_Ctrl_Pin, GPIO_PIN_RESET);
}
