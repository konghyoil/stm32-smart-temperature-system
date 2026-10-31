#include "Fnd.h"

const unsigned char fnd_font[] =
{
		0xC0,
		0xF9,
		0xA4,
		0xB0,
		0x99,
		0x92,
		0x82,
		0xF8,
		0x80,
		0x90
};

void Adc_data_init(unsigned int * data)
{
	unsigned int temp_init;

	temp_init = Adc_Conversion();

	for(int i = 0; i < 10;i++)
	{
		*(data+i) = temp_init;
	}
}

unsigned int read_adc_average(unsigned int adc_value, unsigned int *data)
{
	static int head = 0;
	unsigned int data_sum = 0;

	data[head] = adc_value;
	head = (head + 1) % 10;

	for(int i = 0; i < 10; i++)
	{
		data_sum += data[i];
	}

	return data_sum / 10;
}

unsigned int Adc_Conversion()
{
	unsigned int adc_value;

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	return adc_value * 60 / 4095 ;
}

void Fnd_print(unsigned int adc_value)
{
	static int cnt = 0;

	int Fnd_Data = adc_value;

	int n_10 = Fnd_Data % 100 / 10;
	int n_1 = Fnd_Data % 10;

	if(cnt == 0 && (n_10 >= 1 && n_10 <= 10))
	{
		if (Fnd_print_char(fnd_font[n_10], 0b0010) == -1) return;
	}
	else
	{
		if (Fnd_print_char(fnd_font[n_1], 0b0001) == -1) return;
	}

 	cnt++;
	cnt = (cnt % 2);
}

int Fnd_print_char(unsigned char tx, unsigned char port)
{
	HAL_SPI_Transmit(&hspi2, &tx, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi2, &port, 1, HAL_MAX_DELAY);

	latch();

	return 1;
}

void latch()
{
	HAL_GPIO_WritePin(FND_RCK_GPIO_Port, FND_RCK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(FND_RCK_GPIO_Port, FND_RCK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FND_RCK_GPIO_Port, FND_RCK_Pin, GPIO_PIN_SET);
}


