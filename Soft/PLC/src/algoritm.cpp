#include "algoritm.h"
#include "arduino.h"
#include "analog.h"
#include "define.h"
#include "cells.h"

#define _STEP_INIT                  0
#define _STEP_AVTOMAT_OFF           1
#define _STEP_AVTOMAT_ON_PUMP_OFF   2
#define _STEP_AVTOMAT_ON_PUMP_ON    3
#define _STEP_ERROR                 4

void cAlgoritm()
{
  switch (_ALG_STEP)
  {
    case _STEP_INIT:
      _ALG_STEP = _STEP_AVTOMAT_OFF;
      break;
    case _STEP_AVTOMAT_OFF:
      if (1/* condition */) {
        /* code */
      }
      break;
    case _STEP_AVTOMAT_ON_PUMP_OFF:
      break;
    case _STEP_AVTOMAT_ON_PUMP_ON:
      break;
    case _STEP_ERROR:
      break;

  };

  digitalWrite(_DIS_RELE_K1, cellBool[0].var);
  digitalWrite(_DIS_RELE_K2, cellBool[1].var);
  digitalWrite(_DIS_RELE_K3, cellBool[2].var);
  digitalWrite(_DIS_RELE_K4, cellBool[3].var);
  digitalWrite(_DIS_RELE_K5, cellBool[4].var);

  // Расчет тока
  if (cellBool[20].var == 1){
    float tempSumm = 0.0, tempCurrent = 0.0;
    for (size_t i = 0; i < COUNT_CURRENT_ARRAY; i++){
      tempSumm += (float)currentArray[i];
    }
    float tempAver = tempSumm / COUNT_CURRENT_ARRAY;
    uint16_t tempAverInt = (uint16_t) tempAver;
    for (size_t i = 0; i < COUNT_CURRENT_ARRAY; i++){
      tempCurrent += tempAverInt > currentArray[i]
                    ? tempAver - (float)currentArray[i]
                    : (float)currentArray[i] -tempAver;
    }
    _CURRENT_IN = (uint16_t)tempCurrent;
    cellBool[20].var = 0;

  }


}
