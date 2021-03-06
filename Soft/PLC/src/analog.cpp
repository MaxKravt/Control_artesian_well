#include "analog.h"
#include "cells.h"
#include "pins_arduino.h"
#include "define.h"

int16_t currentArray[COUNT_CURRENT_ARRAY];
int16_t pressureArrayMed[COUNT_PRESSURE_ARRAY_MED];
static int16_t  pointCurrentArray = 0, pointPressureArrayMed=0, anaStep = 0;

void cAdcProc ()
{
  static uint8_t fstCycl = 1;
  if (fstCycl == 1){
    fstCycl = 0;
    for (size_t i = 0; i < COUNT_CURRENT_ARRAY; i++) {
      MB_Reg[i] = (uint16_t *)&currentArray[i];
    }
  }

  switch (anaStep) {
    case 0:{
      analogReference(EXTERNAL); // init analog
      anaStep = 1;
      break;
    }
    case 1:{
      _PRESS_ADC_IN  = analogRead(_ANA_PRESS_PUMP);
      pointPressureArrayMed = pointPressureArrayMed < (COUNT_PRESSURE_ARRAY_MED - 1)
                          ? pointPressureArrayMed + 1
                          :0;
      pressureArrayMed[pointPressureArrayMed] = _PRESS_ADC_IN;
      int16_t arrayPressureValMax = pressureArrayMed[0];
      int16_t arrayPressureValMin = pressureArrayMed[0];
      size_t arrayPressureIndxMax = 0;
      size_t arrayPressureIndxMin = 0;

      for (size_t i = 1; i < COUNT_PRESSURE_ARRAY_MED; i++) {
        if (pressureArrayMed[i] > arrayPressureValMax) {
          arrayPressureValMax = pressureArrayMed[i];
          arrayPressureIndxMax = i;
        }
        if (pressureArrayMed[i] < arrayPressureValMin) {
          arrayPressureValMin = pressureArrayMed[i];
          arrayPressureIndxMin = i;
        }
      }
      int16_t tempSumm = 0;
      int16_t tempSummCount = 0;
      for (size_t i = 0; i < COUNT_PRESSURE_ARRAY_MED; i++) {
        if(i != arrayPressureIndxMax && i != arrayPressureIndxMin){
          tempSumm += pressureArrayMed[i];
          tempSummCount ++;
        }
      }
      int16_t tempMed = tempSumm / tempSummCount;
      _PRESS_IN      = (int16_t)((float)(_VAL_MAX_A0 - _VAL_MIN_A0)
                      / (float)(_ADC_MAX_A0 -_ADC_MIN_A0)
                      * (float)(tempMed - _ADC_MIN_A0)
                      + (float)_VAL_MIN_A0);
      analogReference(DEFAULT); // init analog
      anaStep = 2;
      break;
    }
    case 2:{
      _CURRENT_ADC_IN = analogRead(_ANA_CURRENT_PUPM);
      pointCurrentArray = pointCurrentArray < (COUNT_CURRENT_ARRAY  - 1) ? pointCurrentArray + 1 :0;
      if(cellBool[20].var == 0)
        currentArray[pointCurrentArray] = _CURRENT_ADC_IN;
      if (pointCurrentArray == (COUNT_CURRENT_ARRAY  - 1)) {
        anaStep = 3;
        cellBool[20].var = 1; // Готовность данных для расчета тока
        analogReference(EXTERNAL); // init analog

      }
      break;
    }
    case 3:{
      static uint8_t timeout= 0;
      if (timeout > 15) {
        anaStep = 1;
        timeout = 0;
      }
      else {
        timeout ++;
        analogRead(_ANA_PRESS_PUMP);
      }
      break;
    }
  }




//  digitalWrite(LED_BUILTIN, cellBool[2].var);


}
