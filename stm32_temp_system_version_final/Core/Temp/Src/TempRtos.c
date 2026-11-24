#include "TempRtos.h"

SemaphoreHandle_t SystemDataMutex = NULL;
SemaphoreHandle_t UartRxsemaphore = NULL;
SystemData_t Data;

void Task_Init()
{
	SystemDataMutex = xSemaphoreCreateMutex();
	UartRxsemaphore = xSemaphoreCreateBinary();

	int a, b, c, d, e;

	a = xTaskCreate(Display_Task, "Display_Task", 128, NULL, 2, NULL);
	b = xTaskCreate(Sensor_Task, "Sensor_Task", 256, NULL, 7, NULL);
	c = xTaskCreate(Control_Task, "Control_Task", 128, NULL, 6, NULL);
	d = xTaskCreate(UartTx_Task, "UartTx_Task", 192, NULL, 4, NULL);
	e = xTaskCreate(UartRx_Task, "UartRx_Task", 192, NULL, 5, NULL);

	vTaskStartScheduler();
}

void Display_Task(void* argument)
{
	  char buf[20];
	  SystemData_t data;

	  ssd1306_Init(&hi2c1);
	  display_Init();

	  while(1)
	  {
		  Data_read(&data);

		  display_print(&data);

		  vTaskDelay(pdMS_TO_TICKS(1000));
	  }
}

void Sensor_Task(void* argument)
{
	SystemData_t system_data;
	dht11_data sensor_data;

	dht11_init(GPIOB, GPIO_PIN_4, &htim1);

	while(1)
	{
		sensor_data = dht11_data_read();

		if (!sensor_data.error)
		{
			Data_read(&system_data);

		    system_data.temp_current = sensor_data.temperature;

		    Data_write(system_data);
		}

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void Control_Task(void* argument)
{
	SystemData_t data;

	while(1)
	{
		Data_read(&data);

		data.state = state_machine(&data);
		gpio_control(data.state);

		Data_write(data);

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void UartTx_Task(void* argument)
{
	SystemData_t data;

	while(1)
	{
		Data_read(&data);

		transmit_data(&data);

		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void UartRx_Task(void* argument)
{
	UART_RX_Start();
	float target;

	while(1)
	{
		xSemaphoreTake(UartRxsemaphore, portMAX_DELAY);

		if (strncmp((char *)rx_buf, "POWER ", 6) == 0)
		{
			UART_RX_ProcessPower((char *)rx_buf);
		}
		else if (strncmp((char *)rx_buf, "TEMP ", 5) == 0)
		{
			target = UART_RX_ParseTemp((char*)rx_buf);

			if (target >= 0.0f)
			{
				xSemaphoreTake(SystemDataMutex, portMAX_DELAY);
				Data.temp_target = target;
				xSemaphoreGive(SystemDataMutex);
			}
		}
	}
}

void Data_read(SystemData_t* data)
{
	xSemaphoreTake(SystemDataMutex, portMAX_DELAY);
	*data = Data;
	xSemaphoreGive(SystemDataMutex);
}

void Data_write(SystemData_t data)
{
	xSemaphoreTake(SystemDataMutex, portMAX_DELAY);
	Data = data;
	xSemaphoreGive(SystemDataMutex);
}
