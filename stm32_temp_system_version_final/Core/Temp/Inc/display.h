#ifndef DISPLAY_H
#define DISPLAY_H

#include "main.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ssd1306.h"
#include "fonts.h"
#include "Shared_Resource.h"
#include "control.h"

void display_Init();
void display_print(SystemData_t* data);

#endif
