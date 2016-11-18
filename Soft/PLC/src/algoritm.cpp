#include "algoritm.h"
#include "arduino.h"
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


}
