#ifndef STATE_H
#define STATE_H

#include "FreeRTOS.h"
#include "main.h"
#include "semphr.h"

#include "Temp_FreeRtos.h"
#include "Gpio.h"

#define button_on 1

#define MaxTemp 60
#define MinTemp -50

typedef enum{
	OFF_STATE,
	IDLE_STATE,
	HEATING_STATE,
	ERROR_STATE
}Temperature_State;

typedef struct{
	Temperature_State State;
	int temp_target;
	float temp_current;
	char button_flag;
}Temperature_data;

extern Temperature_data System_Data;

void Set_State(Temperature_data *Set_Data);
void State_Work(Temperature_data *Work_Data);

#endif
