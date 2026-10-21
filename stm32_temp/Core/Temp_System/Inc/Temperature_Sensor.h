#ifndef TEMPERTURE_SENSOR_H
#define TEMPERTURE_SENSOR_H

#include "FreeRTOS.h"
#include "main.h"
#include "Task.h"


void delay_us(int us);

void Ds_Output();
void Ds_Input();
unsigned char Ds_reset();
void Ds_WriteBit(unsigned char bit);
void Ds_WriteByte(unsigned char data);
unsigned char Ds_ReadBit();
unsigned char Ds_ReadByte();

float Temperature_read();


#endif
