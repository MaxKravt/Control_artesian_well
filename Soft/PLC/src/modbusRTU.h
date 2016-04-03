

#ifndef MODBUSRTU_H
#define MODBUSRTU_H
#include "arduino.h"
#include "cells.h"
#include "mbPort.h"




class cModbus
{
private:

  uint8_t modbus(void);
  uint16_t CRC16(uint8_t *, uint16_t );
  void MB_0x01(struct sFrame * );
  void MB_0x02(struct sFrame * );
  void MB_0x03(struct sFrame * );
  void MB_0x04(struct sFrame * );
  void MB_0x05(struct sFrame * );
  void MB_0x06(struct sFrame * );
  void MB_0x07(struct sFrame * );
  void MB_0x08(struct sFrame * );
  void MB_0x0B(struct sFrame * );
  void MB_0x0C(struct sFrame * );
  void MB_0x0F(struct sFrame * );
  void MB_0x10(struct sFrame * );
  void MB_0x11(struct sFrame * );
  void MB_0x14(struct sFrame * );
  void MB_0x15(struct sFrame * );
  void MB_0x16(struct sFrame * );
  void MB_0x17(struct sFrame * );
  void MB_0x18(struct sFrame * );
public:
  void begin(HardwareSerial port, uint16_t boudRate );
  void task();
  void mbRtu(struct sFrame * );
};
extern cModbus modbus;
#endif
