#include "analog.h"
#include "cells.h"
#include "pins_arduino.h"
#include "define.h"

uint16_t currentArray[COUNT_CURRENT_ARRAY];
uint16_t pressureArray[COUNT_PRESSURE_ARRAY];
static uint16_t  pointCurrentArray = 0, pointPressureArray=0, anaStep = 0;
void cAdcProc ()
{
  static uint8_t fstCycl = 1;
  if (fstCycl == 1){
    fstCycl = 0;
    MB_Reg[0] = &currentArray[0];
    MB_Reg[1] = &currentArray[1];
    MB_Reg[2] = &currentArray[2];
    MB_Reg[3] = &currentArray[3];
    MB_Reg[4] = &currentArray[4];
    MB_Reg[5] = &currentArray[5];
    MB_Reg[6] = &currentArray[6];
    MB_Reg[7] = &currentArray[7];
    MB_Reg[8] = &currentArray[8];
    MB_Reg[9] = &currentArray[9];
    MB_Reg[10] = &currentArray[10];
    MB_Reg[11] = &currentArray[11];
    MB_Reg[12] = &currentArray[12];
    MB_Reg[13] = &currentArray[13];
    MB_Reg[14] = &currentArray[14];
    MB_Reg[15] = &currentArray[15];
    MB_Reg[16] = &currentArray[16];
    MB_Reg[17] = &currentArray[17];
    MB_Reg[18] = &currentArray[18];
    MB_Reg[19] = &currentArray[19];
  }

  switch (anaStep) {
    case 0:{
      analogReference(false); // init analog
      anaStep = 1;
      break;
    }
    case 1:{
      _PRESS_ADC_IN  = analogRead(_ANA_PRESS_PUMP);
      pointPressureArray = pointPressureArray < (COUNT_PRESSURE_ARRAY - 1)
                          ? pointPressureArray + 1
                          :0;
      pressureArray[pointPressureArray] = _PRESS_ADC_IN;
      uint16_t arrayPressureValMax = pressureArray[0];
      uint16_t arrayPressureValMin = pressureArray[0];
      size_t arrayPressureIndxMax = 0;
      size_t arrayPressureIndxMin = 0;

      for (size_t i = 1; i < COUNT_PRESSURE_ARRAY; i++) {
        if (pressureArray[i] > arrayPressureValMax) {
          arrayPressureValMax = pressureArray[i];
          arrayPressureIndxMax = i;
        }
        if (pressureArray[i] < arrayPressureValMin) {
          arrayPressureValMin = pressureArray[i];
          arrayPressureIndxMin = i;
        }
      }
      uint16_t tempSumm = 0;
      uint16_t tempSummCount = 0;
      for (size_t i = 0; i < COUNT_PRESSURE_ARRAY; i++) {
        if(i != arrayPressureIndxMax && i != arrayPressureIndxMin){
          tempSumm += pressureArray[i];
          tempSummCount ++;
        }
      }
      uint16_t tempMed = tempSumm / tempSummCount;
      _PRESS_IN      = (int16_t)((float)(_VAL_MAX_A0 - _VAL_MIN_A0)
                      / (float)(_ADC_MAX_A0 -_ADC_MIN_A0)
                      * (float)(tempMed - _ADC_MIN_A0)
                      + (float)_VAL_MIN_A0);
      analogReference(true); // init analog
      anaStep = 2;
      break;
    }
    case 2:{
      _CURRENT_ADC_IN = analogRead(_ANA_CURRENT_PUPM);
      pointCurrentArray = pointCurrentArray < (COUNT_CURRENT_ARRAY  - 1) ? pointCurrentArray + 1 :0;
      if(cellBool[20].var == 0)
        currentArray[pointCurrentArray] = _CURRENT_ADC_IN;
      if (pointCurrentArray == (COUNT_CURRENT_ARRAY  - 1)) {
        anaStep = 1;
        cellBool[20].var = 1; // Готовность данных для расчета тока
        analogReference(false); // init analog
      }
      break;
    }
  }




//  digitalWrite(LED_BUILTIN, cellBool[2].var);


}
