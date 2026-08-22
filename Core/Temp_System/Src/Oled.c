#include "Oled.h"

const unsigned char font[][5] =
{
/* SPACE */
{0x00,0x00,0x00,0x00,0x00},

/* ! */
{0x00,0x00,0x5F,0x00,0x00},

/* " */
{0x00,0x07,0x00,0x07,0x00},

/* # */
{0x14,0x7F,0x14,0x7F,0x14},

/* $ */
{0x24,0x2A,0x7F,0x2A,0x12},

/* % */
{0x23,0x13,0x08,0x64,0x62},

/* & */
{0x36,0x49,0x55,0x22,0x50},

/* ' */
{0x00,0x05,0x03,0x00,0x00},

/* ( */
{0x00,0x1C,0x22,0x41,0x00},

/* ) */
{0x00,0x41,0x22,0x1C,0x00},

/* * */
{0x14,0x08,0x3E,0x08,0x14},

/* + */
{0x08,0x08,0x3E,0x08,0x08},

/* , */
{0x00,0x50,0x30,0x00,0x00},

/* - */
{0x08,0x08,0x08,0x08,0x08},

/* . */
{0x00,0x60,0x60,0x00,0x00},

/* / */
{0x20,0x10,0x08,0x04,0x02},

/* 0 */
{0x3E,0x51,0x49,0x45,0x3E},

/* 1 */
{0x00,0x42,0x7F,0x40,0x00},

/* 2 */
{0x42,0x61,0x51,0x49,0x46},

/* 3 */
{0x21,0x41,0x45,0x4B,0x31},

/* 4 */
{0x18,0x14,0x12,0x7F,0x10},

/* 5 */
{0x27,0x45,0x45,0x45,0x39},

/* 6 */
{0x3C,0x4A,0x49,0x49,0x30},

/* 7 */
{0x01,0x71,0x09,0x05,0x03},

/* 8 */
{0x36,0x49,0x49,0x49,0x36},

/* 9 */
{0x06,0x49,0x49,0x29,0x1E},

/* : */
{0x00,0x36,0x36,0x00,0x00},

/* ; */
{0x00,0x56,0x36,0x00,0x00},

/* < */
{0x08,0x14,0x22,0x41,0x00},

/* = */
{0x14,0x14,0x14,0x14,0x14},

/* > */
{0x00,0x41,0x22,0x14,0x08},

/* ? */
{0x02,0x01,0x51,0x09,0x06},

/* @ */
{0x32,0x49,0x79,0x41,0x3E},

/* A */
{0x7E,0x11,0x11,0x11,0x7E},

/* B */
{0x7F,0x49,0x49,0x49,0x36},

/* C */
{0x3E,0x41,0x41,0x41,0x22},

/* D */
{0x7F,0x41,0x41,0x22,0x1C},

/* E */
{0x7F,0x49,0x49,0x49,0x41},

/* F */
{0x7F,0x09,0x09,0x09,0x01},

/* G */
{0x3E,0x41,0x49,0x49,0x7A},

/* H */
{0x7F,0x08,0x08,0x08,0x7F},

/* I */
{0x00,0x41,0x7F,0x41,0x00},

/* J */
{0x20,0x40,0x41,0x3F,0x01},

/* K */
{0x7F,0x08,0x14,0x22,0x41},

/* L */
{0x7F,0x40,0x40,0x40,0x40},

/* M */
{0x7F,0x02,0x0C,0x02,0x7F},

/* N */
{0x7F,0x04,0x08,0x10,0x7F},

/* O */
{0x3E,0x41,0x41,0x41,0x3E},

/* P */
{0x7F,0x09,0x09,0x09,0x06},

/* Q */
{0x3E,0x41,0x51,0x21,0x5E},

/* R */
{0x7F,0x09,0x19,0x29,0x46},

/* S */
{0x46,0x49,0x49,0x49,0x31},

/* T */
{0x01,0x01,0x7F,0x01,0x01},

/* U */
{0x3F,0x40,0x40,0x40,0x3F},

/* V */
{0x1F,0x20,0x40,0x20,0x1F},

/* W */
{0x7F,0x20,0x18,0x20,0x7F},

/* X */
{0x63,0x14,0x08,0x14,0x63},

/* Y */
{0x07,0x08,0x70,0x08,0x07},

/* Z */
{0x61,0x51,0x49,0x45,0x43}
};

void OLED_Init()
{
	OLED_cmd(0xAE);

	OLED_cmd(0x20);
	OLED_cmd(0x10);

	OLED_cmd(0xB0);

	OLED_cmd(0xC8);
	OLED_cmd(0x00);
	OLED_cmd(0x10);

	OLED_cmd(0x40);

	OLED_cmd(0x81);
	OLED_cmd(0xFF);

	OLED_cmd(0xA1);
	OLED_cmd(0xA6);

	OLED_cmd(0xA8);
	OLED_cmd(0x3F);

	OLED_cmd(0xA4);

	OLED_cmd(0xD3);
	OLED_cmd(0x00);

	OLED_cmd(0xD5);
	OLED_cmd(0xF0);

	OLED_cmd(0xD9);
	OLED_cmd(0x22);

	OLED_cmd(0xDA);
	OLED_cmd(0x12);

	OLED_cmd(0xDB);
    OLED_cmd(0x20);

    OLED_cmd(0x8D);
    OLED_cmd(0x14);

    OLED_cmd(0xAF);

    OLED_Clear();

    OLED_set_cursor(0, 0);
}

void OLED_Temp_Init()
{
	OLED_print_string("SMART TEMP SYSTEM");

	OLED_set_cursor(3, 0);

	OLED_print_string("STATE :");

	OLED_set_cursor(3, 48);

	OLED_print_string("OFF");

	OLED_set_cursor(5, 0);

	OLED_print_string("SET TEMP :");

	OLED_set_cursor(7, 0);

	OLED_print_string("CURRENT TEMP :");
}

HAL_StatusTypeDef OLED_cmd(char cmd)
{
	uint8_t tx[2];

	tx[0] = 0x00;
	tx[1] = cmd;

	return HAL_I2C_Master_Transmit(&hi2c2, 0x3C<<1, tx, sizeof(tx), 100);
}

HAL_StatusTypeDef OLED_data(char data)
{
	uint8_t tx[2];

	tx[0] = 0x40;
	tx[1] = data;

	return HAL_I2C_Master_Transmit(&hi2c2, 0x3C<<1, tx, sizeof(tx), 100);
}

void OLED_Clear()
{
	for(int page = 0; page <8; page++)
	{
		OLED_cmd(0xB0+page);

		OLED_cmd(0x00);
		OLED_cmd(0x10);

		for(int i = 0; i<128; i++)
		{
			OLED_data(0x00);
		}
	}
}

HAL_StatusTypeDef OLED_set_cursor(int page, int col)
{
	HAL_StatusTypeDef ret;

	ret = OLED_cmd(0xB0 + page);
	if(ret != HAL_OK) return ret;

	ret = OLED_cmd(0x00 + (col & 0x0F));
	if(ret != HAL_OK) return ret;

	ret = OLED_cmd(0x10 + (col >> 4));
	if(ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef OLED_print_char(char ch)
{
	HAL_StatusTypeDef ret;

	if(ch < 32 || ch > 90)
	{
	    ch = ' ';
	}

	for(int i =0; i<5;i++)
	{
		ret = OLED_data(font[ch - 32][i]);
		if(ret != HAL_OK) return ret;
	}

	ret = OLED_data(0x00);
	if(ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef OLED_print_string(char *str)
{
	HAL_StatusTypeDef ret;

	while(*str)
	{
		ret = OLED_print_char(*str);
		if(ret != HAL_OK) return ret;
		str++;
	}

	return HAL_OK;
}

HAL_StatusTypeDef OLED_State(unsigned int state)
{
	static unsigned int prev_state =  1;
	HAL_StatusTypeDef ret;

	if(prev_state != state)
	{
		ret = OLED_set_cursor(3, 48);
		if(ret != HAL_OK) return ret;

		for(int i = 48; i<120; i++)
		{
			ret = OLED_data(0x00);
			if(ret != HAL_OK) return ret;
		}

		ret = OLED_set_cursor(3, 48);
		if(ret != HAL_OK) return ret;

		if(state == OFF_STATE)
		{
				ret = OLED_print_string("OFF");
				if(ret != HAL_OK) return ret;
		}
		else if(state == IDLE_STATE)
		{
				ret = OLED_print_string("IDLE");
				if(ret != HAL_OK) return ret;
		}
		else if(state == HEATING_STATE)
		{
				ret = OLED_print_string("HEATING");
				if(ret != HAL_OK) return ret;
		}
		else
		{
				ret = OLED_print_string("ERROR");
				if(ret != HAL_OK) return ret;
		}

		prev_state = state;
	}

	return HAL_OK;
}

HAL_StatusTypeDef OLED_Set(int adc_value)
{
	char set_temp[3];
	static unsigned int prev_set_value = 1;
	HAL_StatusTypeDef ret;

	ret = OLED_set_cursor(5, 66);
	if(ret != HAL_OK) return ret;

	if(adc_value > 55)
	{
		ret = OLED_print_string("ERR");
		if(ret != HAL_OK) return ret;
		else return HAL_OK;
	}

	if(prev_set_value != adc_value)
	{
		set_temp[0] = adc_value / 10 % 10 + '0';
		set_temp[1] = adc_value % 10 + '0';
		set_temp[2] = '\0';

		ret = OLED_print_string(set_temp);
		if(ret != HAL_OK) return ret;

		prev_set_value = adc_value;
	}

	return HAL_OK;
}

HAL_StatusTypeDef OLED_Current(float temperature)
{
	char current_temp[5];
	static float prev_temperature = 1.0f;
	HAL_StatusTypeDef ret;

	ret = OLED_set_cursor(7, 90);
	if(ret != HAL_OK) return ret;

	if(temperature > 55)
	{
		ret = OLED_print_string("ERR");
		if(ret != HAL_OK) return ret;
		else return HAL_OK;
	}

	if(prev_temperature != temperature)
	{
		int temp_10 = temperature * 10;

		current_temp[0] = temp_10 / 100 + '0';
		current_temp[1] = temp_10 / 10 % 10 + '0';
		current_temp[2] = '.';
		current_temp[3] = temp_10 % 10 + '0';
		current_temp[4] = '\0';

		ret = OLED_print_string(current_temp);
		if(ret != HAL_OK) return ret;

		prev_temperature = temperature;
	}

	return HAL_OK;
}

HAL_StatusTypeDef OLED_Update(unsigned int state, int target, float current)
{
	HAL_StatusTypeDef ret;

	ret = OLED_State(state);
	if(ret != HAL_OK) return ret;

	ret = OLED_Set(target);
	if(ret != HAL_OK) return ret;

	ret = OLED_Current(current);
	if(ret != HAL_OK) return ret;

	return HAL_OK;
}
