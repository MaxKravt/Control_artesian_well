#include "analog.h"
#include "cells.h"
#include "arduino.h"
#include "pins_arduino.h"


#define pinPres A0

void cAdcProc ()
{
  cell[0].sl = (long)analogRead(pinPres);
  cell[1].bit00 = digitalRead(1);
  cell[2].bit01 = 1;
  
}

