#include "analog.h"
#include "cells.h"
#include "arduino.h"
#include "pins_arduino.h"
#include "define.h"

#define pinPres A0

void cAdcProc ()
{
  cellAna[0].var = analogRead(pinPres);
  cellAna[1].var = (int16_t)((float)(_VAL_MAX_A0 - _VAL_MIN_A0) / (float)(_ADC_MAX_A0 -_ADC_MIN_A0) * (float)(cellAna[0].var - _ADC_MIN_A0)  + (float)_VAL_MIN_A0);

  digitalWrite(_DIS_RELE_K1, cellBool[0].var);
  digitalWrite(_DIS_RELE_K2, cellBool[1].var);
  digitalWrite(_DIS_RELE_K3, cellBool[2].var);
  digitalWrite(_DIS_RELE_K4, cellBool[3].var);
  digitalWrite(_DIS_RELE_K5, cellBool[4].var);


//  digitalWrite(LED_BUILTIN, cellBool[2].var);


}
