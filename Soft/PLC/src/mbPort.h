

#ifndef MBPORT_H
#define MBPORT_H
#include "arduino.h"
#include "modbusRTU.h"

#define MB_BUFFER_SIZE    50

struct sFrame{
  uint8_t buffer[MB_BUFFER_SIZE];
  uint16_t len;
  HardwareSerial * pPort;
  enum eStatus  {
    empty,
    readyToMB,
    readyToPort
  } status;
} ;
extern struct sFrame frame;
extern uint16_t lastTimeByteRx;

void port2Task();
void callbackUart2Tx();
void callbackUart2Rx();
void callbackTimer();
void port2BeginTask();
struct sFrame * getReadyToMbBuffer();

#endif
