#include "display.h"

void display_Init()
{
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("power: off", Font_11x18, White);

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("state: off", Font_11x18, White);

	ssd1306_SetCursor(0, 40);
	ssd1306_WriteString("temp: 0", Font_11x18, White);

	ssd1306_UpdateScreen(&hi2c1);

}

void display_print(SystemData_t* data)
{
	static SystemData_t prev_data = {0};
	static uint8_t prev_flag = 0;
	char buf[20];

	if(flag != prev_flag)
	{
		ssd1306_SetCursor(77, 0);
		ssd1306_WriteString("   ", Font_11x18, White);

		if(flag)
		{
			sprintf(buf,"on");
			ssd1306_SetCursor(77, 0);
			ssd1306_WriteString(buf, Font_11x18, White);
		}
		else
		{
			sprintf(buf,"off");
			ssd1306_SetCursor(77, 0);
			ssd1306_WriteString(buf, Font_11x18, White);
		}
	}

	if(data ->state != prev_data.state)
	{
		ssd1306_SetCursor(77, 20);
		ssd1306_WriteString("    ", Font_11x18, White);

		if(data ->state == STATE_OFF)
		{
			sprintf(buf,"off");
			ssd1306_SetCursor(77, 20);
			ssd1306_WriteString(buf, Font_11x18, White);
		}
		else if(data ->state == STATE_IDLE)
		{
			sprintf(buf,"idle");
			ssd1306_SetCursor(77, 20);
			ssd1306_WriteString(buf, Font_11x18, White);
		}
		else if(data ->state == STATE_HEATING)
		{
			sprintf(buf,"heat");
			ssd1306_SetCursor(77, 20);
			ssd1306_WriteString(buf, Font_11x18, White);
		}
		else
		{
			sprintf(buf,"err");
			ssd1306_SetCursor(77, 20);
			ssd1306_WriteString(buf, Font_11x18, White);
		}
	}

	if(data ->temp_current != prev_data.temp_current)
	{
		ssd1306_SetCursor(66, 40);
		ssd1306_WriteString("   ", Font_11x18, White);

		sprintf(buf, "%d.%d", (int)data->temp_current, ((int)(data->temp_current * 10) % 10));
		ssd1306_SetCursor(66, 40);
		ssd1306_WriteString(buf, Font_11x18, White);
	}

	ssd1306_UpdateScreen(&hi2c1);

	prev_data = *data;
	prev_flag = flag;
}
