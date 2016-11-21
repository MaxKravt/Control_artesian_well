
#ifndef ALGORITM_H_
#define ALGORITM_H_

#include <Arduino.h>

void cAlgoritm ();

// Функция TON
struct sTimerOn {
  uint32_t acc;
  uint32_t time;
  uint8_t in:1;
  uint8_t run:1;
  uint8_t out:1;
};

void timeON(sTimerOn * data);

#endif
