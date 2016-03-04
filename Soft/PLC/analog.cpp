#include "analog.h"
#include "arduino.h"
#include "pins_arduino.h"

#define pinPres A0

long cell[10];

void cAdcProc ()
{
  cell[0] = (long)analogRead(pinPres);
  
}

