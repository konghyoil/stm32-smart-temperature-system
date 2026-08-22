#ifndef FND_H
#define FND_H

#include "main.h"

extern const unsigned char fnd_font[];

void Adc_data_init(unsigned int * data);
unsigned int Adc_Conversion();
unsigned int read_adc_average(unsigned int adc_value, unsigned int *data);
void Fnd_print(unsigned int adc_value);
int Fnd_print_char(unsigned char tx, unsigned char port);
void latch();

#endif
