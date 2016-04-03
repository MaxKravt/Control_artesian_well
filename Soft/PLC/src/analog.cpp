#include "analog.h"
#include "cells.h"
#include "arduino.h"
#include "pins_arduino.h"


#define pinPres A0

void cAdcProc ()
{
  cellAna[0].var = analogRead(pinPres);


  cellBool[0].var = digitalRead(1);
  digitalWrite(LED_BUILTIN, cellBool[2].var);


}
