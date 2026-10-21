#ifndef GPIO_H
#define GPIO_H

#include "Temp_FreeRtos.h"
#include "main.h"
#include "semphr.h"

#include "Temp_FreeRtos.h"
#include "State.h"

//led, switch, relay header file

int Led_onoff(int button_flag);
void Relay_on();
void Relay_off();

#endif
