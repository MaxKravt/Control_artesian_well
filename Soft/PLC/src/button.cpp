#include "button.h"
#include "arduino.h"
#include "define.h "
#include "cells.h"

void cButton()
{
  static uint8_t buttonTOF [3] = {0,0,0};
  #define  buttonMask  0b1111
  buttonTOF[0] = (buttonTOF[0] << 1) + digitalRead(_DIS_BUTTTON_1);
  buttonTOF[1] = (buttonTOF[1] << 1) + digitalRead(_DIS_BUTTTON_2);
  buttonTOF[2] = (buttonTOF[2] << 1) + digitalRead(_DIS_BUTTTON_3);

  _BUTTON_UP.var = (buttonTOF[0] & buttonMask) == 0 ? 0: _BUTTON_UP.var;
  _BUTTON_UP.var = (buttonTOF[0] & buttonMask) == buttonMask ? 1: _BUTTON_UP.var;

  _BUTTON_DWN.var = (buttonTOF[1] & buttonMask) == 0 ? 0: _BUTTON_DWN.var;
  _BUTTON_DWN.var = (buttonTOF[1] & buttonMask) == buttonMask ? 1: _BUTTON_DWN.var;

  _BUTTON_RGHT.var = (buttonTOF[2] & buttonMask) == 0 ? 0: _BUTTON_RGHT.var;
  _BUTTON_RGHT.var = (buttonTOF[2] & buttonMask) == buttonMask ? 1: _BUTTON_RGHT.var;


}
