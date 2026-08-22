#include "Temp_FreeRtos.h"

SemaphoreHandle_t ButtonSemaphore;
SemaphoreHandle_t State_Mutex;

void Semaphore_Init()
{
	ButtonSemaphore = xSemaphoreCreateBinary();
}

void Mutex_Init()
{
	State_Mutex = xSemaphoreCreateMutex();
}

void FreeRtos_Init()
{
	configASSERT(xTaskCreate(Led_Task, "led", 64, NULL, 6, NULL) == pdPASS);
	configASSERT(xTaskCreate(Fnd_Task, "fnd", 64, NULL, 4, NULL) == pdPASS);
	configASSERT(xTaskCreate(Temp_Task, "temp", 64, NULL, 5, NULL) == pdPASS);
	configASSERT(xTaskCreate(State_Task, "state", 64, NULL, 3, NULL) == pdPASS);
	configASSERT(xTaskCreate(Uart_Task, "uart", 256, NULL, 1, NULL) == pdPASS);
	configASSERT(xTaskCreate(Oled_Task, "oled", 256, NULL, 2, NULL) == pdPASS);
}

void Led_Task(void * argument)
{
	int button_flag = 0;

	while(1)
	{
		button_flag = Led_onoff(button_flag);
	}
}

void Fnd_Task(void *argument)
{
	unsigned int adc_value;
	unsigned int adc_data[10];

	Adc_data_init(adc_data);

	while(1)
	{
		adc_value = Adc_Conversion();

		adc_value = read_adc_average(adc_value, adc_data);

		Fnd_print(adc_value);

		xSemaphoreTake(State_Mutex, portMAX_DELAY);
		System_Data.temp_target = adc_value;
		xSemaphoreGive(State_Mutex);

		vTaskDelay(pdMS_TO_TICKS(7));
	}
}

void Temp_Task(void *argument)
{
	float Temp_current = 0.0f;

	while(1)
	{
		Temp_current = Temperature_read();

		xSemaphoreTake(State_Mutex, portMAX_DELAY);
		System_Data.temp_current = Temp_current;
		xSemaphoreGive(State_Mutex);

		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void State_Task(void *argument)
{
	Temperature_data state_data;

	while(1)
	{
		xSemaphoreTake(State_Mutex, portMAX_DELAY);
		state_data = System_Data;
		xSemaphoreGive(State_Mutex);

		Set_State(&state_data);
		State_Work(&state_data);

	    xSemaphoreTake(State_Mutex, portMAX_DELAY);
	    System_Data = state_data;
	    xSemaphoreGive(State_Mutex);

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void Uart_Task(void *argument)
{
	Temperature_data uart_data;
	int ret;

	while (1)
	{
		xSemaphoreTake(State_Mutex, portMAX_DELAY);
		uart_data = System_Data;
		xSemaphoreGive(State_Mutex);

		ret = Uart_work(uart_data.State, uart_data.temp_target, uart_data.temp_current);

		if(ret != HAL_OK)
		{
			vTaskDelay(pdMS_TO_TICKS(10));

			ret = Uart_work(uart_data.State, uart_data.temp_target, uart_data.temp_current);

			if(ret != HAL_OK)
			{
				vTaskDelay(pdMS_TO_TICKS(10));

				HAL_UART_Abort(&huart1);
				HAL_UART_DeInit(&huart1);
				HAL_UART_Init(&huart1);

				ret = Uart_work(uart_data.State, uart_data.temp_target, uart_data.temp_current);
			}
		}

		UBaseType_t remain = uxTaskGetStackHighWaterMark(NULL);

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void Oled_Task(void *argument)
{
	Temperature_data oled_data;
	HAL_StatusTypeDef ret;

	OLED_Init();
	OLED_Temp_Init();

	while(1)
	{
		xSemaphoreTake(State_Mutex, portMAX_DELAY);
		oled_data = System_Data;
		xSemaphoreGive(State_Mutex);

		ret = OLED_Update(oled_data.State, oled_data.temp_target, oled_data.temp_current);

		if(ret != HAL_OK)
		{
			vTaskDelay(pdMS_TO_TICKS(10));

			OLED_Init();
			OLED_Temp_Init();

			ret = OLED_Update(oled_data.State, oled_data.temp_target, oled_data.temp_current);

			if(ret != HAL_OK)
			{
				vTaskDelay(pdMS_TO_TICKS(10));

				HAL_I2C_DeInit(&hi2c2);
				HAL_I2C_Init(&hi2c2);

				OLED_Init();
				OLED_Temp_Init();

				ret = OLED_Update(oled_data.State, oled_data.temp_target, oled_data.temp_current);
			}
		}

		UBaseType_t remain = uxTaskGetStackHighWaterMark(NULL);

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}


