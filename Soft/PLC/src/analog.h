
#ifndef ANALOG_H_
#define ANALOG_H_

#include <Arduino.h>
#define COUNT_CURRENT_ARRAY 10
#define COUNT_CURRENT_ARRAY_MED 10
#define COUNT_PRESSURE_ARRAY_MED 20

extern int16_t currentArray[COUNT_CURRENT_ARRAY];
extern int16_t pressureArrayMed[COUNT_PRESSURE_ARRAY_MED];

void cAdcProc ();

#endif
