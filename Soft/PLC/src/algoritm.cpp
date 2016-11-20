/*

      Под управлением САУ находится 5 реле. Для предотвращения выхода из строя
 * оборудования необходимо переключать их с соблюдением условия, что цепь
 * должна быть разорвана. Для лучшего понимания как надо делать напишу таблицу
 * переходов.
 * ____________________________________________
 * |                 |         Реле           |
 * |   Состояние     +------------------------+
 * |                 | R1 | R2 | R3 | R4 | R5 |
 * +-----------------+----+----+----+----+----+
 * | Отключено       | 0  | 0  | 0  | 0  | 0  |
 * +-----------------+----+----+----+----+----+
 * |                 |    |    |    |    |    |
 * +-----------------+----+----+----+----+----+
 * |                 |    |    |    |    |    |
 * +-----------------+----+----+----+----+----+
 * |                 |    |    |    |    |    |
 * +-----------------+----+----+----+----+----+
 * |                 |    |    |    |    |    |
 * +-----------------+----+----+----+----+----+
 * |                 |    |    |    |    |    |
 * +-----------------+----+----+----+----+----+
 * |                 |    |    |    |    |    |
 * +-----------------+----+----+----+----+----+
 *
 */


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



void cAlgoritm(){
  switch (_ALG_STEP)  {
    case _STEP_INIT:
      _ALG_STEP = _STEP_AVTOMAT_OFF;
      break;
    case _STEP_AVTOMAT_OFF:
      if (1) {
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
  digitalWrite(_DIS_LED_SAU, _SAU_WORK);
  digitalWrite(_DIS_LED_WORK,_PUMP_WORK);

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
    _CURRENT_IN = (uint16_t)tempCurrent / COUNT_CURRENT_ARRAY;
    cellBool[20].var = 0;

  }


}
