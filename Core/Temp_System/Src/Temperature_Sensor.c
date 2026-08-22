#include "Temperature_Sensor.h"

void delay_us(int us)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);

	while(__HAL_TIM_GET_COUNTER(&htim2) <= us);
}

void Ds_Output()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = Temp_Sensor_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(Temp_Sensor_GPIO_Port, &GPIO_InitStruct);
}

void Ds_Input()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = Temp_Sensor_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(Temp_Sensor_GPIO_Port, &GPIO_InitStruct);
}

unsigned char Ds_reset()
{
	unsigned char presence;

	Ds_Output();
	HAL_GPIO_WritePin(Temp_Sensor_GPIO_Port, Temp_Sensor_Pin, GPIO_PIN_RESET);

	delay_us(480);

	Ds_Input();

	delay_us(70);

	presence = (HAL_GPIO_ReadPin(Temp_Sensor_GPIO_Port, Temp_Sensor_Pin) == 0);

	delay_us(410);

	return presence; // 1 = error, 0 = ok
}

void Ds_WriteBit(unsigned char bit)
{
	Ds_Output();
	HAL_GPIO_WritePin(Temp_Sensor_GPIO_Port, Temp_Sensor_Pin, GPIO_PIN_RESET);

	if(bit)
	{
		delay_us(6);

		Ds_Input();

		delay_us(64);
	}
	else
	{
		delay_us(60);

		Ds_Input();

		delay_us(10);
	}
}

unsigned char Ds_ReadBit()
{
	unsigned char bit;

	Ds_Output();
	HAL_GPIO_WritePin(Temp_Sensor_GPIO_Port, Temp_Sensor_Pin, GPIO_PIN_RESET);

	delay_us(2);

	Ds_Input();

	delay_us(3);

	bit = (HAL_GPIO_ReadPin(Temp_Sensor_GPIO_Port, Temp_Sensor_Pin) == 1);

	delay_us(60);

	return bit;
}

void Ds_WriteByte(unsigned char data)
{
	for(int i=0; i<8; i++)
	{
		Ds_WriteBit(data & 0x01);
		data >>= 1;
	}
}

unsigned char Ds_ReadByte()
{
	unsigned char data = 0;

	for(int i=0; i<8; i++)
	{
		if(Ds_ReadBit())
		{
			data |= (1<<i);
		}
	}

	return data;
}
float Temperature_read()
{
	unsigned char tempL;
	unsigned char tempH;

	int16_t raw;

	if(!Ds_reset()) return -1000;
	Ds_WriteByte(0xCC);
	Ds_WriteByte(0x44);

	vTaskDelay(pdMS_TO_TICKS(750));

	if(!Ds_reset()) return -1000;
	Ds_WriteByte(0xCC);
	Ds_WriteByte(0xBE);

	tempL = Ds_ReadByte();
	tempH = Ds_ReadByte();

	raw = ((int16_t)tempH << 8) | tempL;

	return (float)raw / 16.0f;
}
