/*
 * cells.h
 *
 * Created: 10.03.2016 11:53:43
 *  Author: Max
 */ 




#ifndef CELLS_H_
#define CELLS_H_
#include "arduino.h"


#define cellCount		30
#define cellRetainStart 20
#define cellRetainCount 10

struct sCell
{
	union						// значение переменной
	{
		long sl;
		unsigned	long ul;
		float fl;
		struct{
			byte byte0:8;
			byte byte1:8;
			byte byte2:8;
			byte byte3:8;
		};

		struct{
			long bit00:1;
			long bit01:1;
			long bit02:1;
			long bit03:1;
		};
	} ;
	long dataOld;				// значение на пршлом циклеконтроля изменения
	long retain;				// признак сохранения в EEPROM
};
extern sCell cell[cellCount];

int cellInit (void);
void cellRun(void);

#endif /* CELLS_H_ */

