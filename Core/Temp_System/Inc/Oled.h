#ifndef OLED_H
#define OLED_H

#include "FreeRTOS.h"
#include "main.h"

#include "State.h"

extern const unsigned char font[][5];

void OLED_Init();
void OLED_Temp_Init();
HAL_StatusTypeDef OLED_cmd(char cmd);
HAL_StatusTypeDef OLED_data(char data);
void OLED_Clear();
HAL_StatusTypeDef OLED_set_cursor(int page, int col);
HAL_StatusTypeDef OLED_print_char(char ch);
HAL_StatusTypeDef OLED_print_string(char *str);
HAL_StatusTypeDef OLED_State(unsigned int state);
HAL_StatusTypeDef OLED_Set(int adc_value);
HAL_StatusTypeDef OLED_Current(float temperature);
HAL_StatusTypeDef OLED_Update(unsigned int state, int target, float current);

#endif
