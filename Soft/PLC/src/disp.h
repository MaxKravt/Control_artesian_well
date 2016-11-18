/*
 * disp.h
 *
 * Created: 11.03.2016 13:17:44
 *  Author: Max
 */



#ifndef DISP_H_
#define DISP_H_
  #include "Arduino.h"

  void cDispRun(void);

  void ledWriteChar (uint8_t addr, uint8_t data );
  void ledInit (void);
  uint8_t ledDecode (uint8_t in);
  void ledWriteString(String in);

  struct sLedCharConfig {
  	uint8_t  index;
  	uint8_t  decod;
  };
#endif /* DISP_H_ */
