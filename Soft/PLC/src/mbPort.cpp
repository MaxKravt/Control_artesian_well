#include "mbPort.h"
#include "string.h"

uint16_t lastTimeByteRx = 0;
uint8_t  dataRx = 0;
struct sFrame frame;

void callbackUart2Tx()
{
  // переключение передатчика RS-485 в режим приема.
}

void callbackUart2Rx()
{
  // замер времени после последнего полученого байта
  dataRx = 1;
  lastTimeByteRx = micros();
}

void port2BeginTask()
{
  Serial2.EventEndTransmit = callbackUart2Tx;
  Serial2.EventReceive = callbackUart2Rx;
}

void port2Task()
{
  static uint8_t state = 0;
  switch (state)
  {
    case 0: // ожидание начала передачи данных
    {
      if (dataRx == 1)
      {
        dataRx = 0;
        state = 1;
      }
      break;
    }
    case 1: // получение данных
    {
      if ((micros() - lastTimeByteRx) > 20 * (1000000/11520))
      {state = 2;}
      break;
    }
    case 2: // окончание пелучения данных
    {
      size_t len = Serial2.available();
      Serial2.readBytes(&frame.buffer[0],  len);
      frame.len = len;
      modbus.task();
      state = 3;
      //Serial.println(String() +"Pack read: Len: "+ String(len));
      break;
    }
    case 3: // Передача данных в порт
    {
      for(uint16_t i= 0; i < frame.len; i++)
        Serial2.write(frame.buffer[i]);
      state = 0;
      dataRx = 0;
      break;
    }
  }
}

struct sFrame * getReadyToMbBuffer()
{
  return & frame;
}
