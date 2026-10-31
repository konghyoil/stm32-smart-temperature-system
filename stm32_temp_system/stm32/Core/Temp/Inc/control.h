#ifndef GPIO_H
#define GPIO_H

#include "main.h"
#include "TempRtos.h"
#include "Shared_Resource.h"
#include <stdint.h>
#include <stdbool.h>

extern volatile uint8_t flag;

SystemState_t state_machine(SystemData_t *data);
void gpio_control(SystemState_t state);

#endif
