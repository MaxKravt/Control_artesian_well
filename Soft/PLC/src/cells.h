/*
 * cells.h
 *
 * Created: 10.03.2016 11:53:43
 *  Author: Max
 */




#ifndef CELLS_H_
#define CELLS_H_
#include "arduino.h"


#define cellCount		    30
#define cellRetainStart 20
#define cellRetainCount 10

#define MAX_CELL_BOOL	  30
#define MAX_CELL_ANA	  30

#define MB_SIZE 	      5

#define CELL_OK			    1
#define CELL_ERROR		  2
#define CELL_LOCK		    3

#define COMM_OK		      10
#define COMM_ERROR		  11
#define COMM_NOT_FOUND	12

struct sCellBool{
	unsigned int var 	  :1;  // значение переменной
	unsigned int var_o  :1;  // значение на прошлом шаге, необходимо для архивации
	unsigned int lock   :1;
	unsigned int retain :1;
};
struct sCellAna{
	union	{
		int16_t var;
		struct {
			uint8_t byte0;
			uint8_t byte1;
			uint8_t byte2;
			uint8_t byte3;
		};
	};
	int16_t var_o;
	uint16_t lock :1;
	uint16_t retain :1;
};


extern sCellAna cellAna[cellCount];
extern sCellBool cellBool[cellCount];

struct sCellBool * Cell_b_get_ptr(void);
struct sCellAna  * Cell_a_get_ptr(void);

int8_t Cell_b_get (struct sCellBool *, 	uint16_t);
int8_t Cell_a_get (struct sCellAna *,  	uint16_t);

int8_t Cell_b_set (uint16_t, 			uint16_t);
int8_t Cell_a_set (uint16_t, 			uint16_t);

uint8_t Cell_b_get_var(uint16_t);
int16_t  Cell_a_get_var(uint16_t);

int8_t Cell_b_set_var(int8_t, uint16_t);
int8_t Cell_a_set_var(int16_t, uint16_t);

int8_t Cell_r_set (float set, uint16_t);


int cellInit (void);
void cellRun(void);

#endif /* CELLS_H_ */
