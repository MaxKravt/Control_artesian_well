/*
 * cells.cpp
 *
 * Created: 10.03.2016 11:53:23
 *  Author: Max
 */ 
#include "cells.h"
#include "eeprom.h"


sCell cell[cellCount];
/* Инициализация ячеек. Чтение из EEPROM */ 

int cellInit (void)
{
	// Чтение из EEPROM
	for (int i=cellRetainStart; i++; i<cellRetainCount)
	{	
		cell[i].retain = 1;
		cell[i].byte0 = EEPROM.read(i * 4 + 0);	
		cell[i].byte1 = EEPROM.read(i * 4 + 1);
		cell[i].byte2 = EEPROM.read(i * 4 + 2);
		cell[i].byte3 = EEPROM.read(i * 4 + 3);
	}
	// инициализация для первого цикла
	for (int i = 0; i++; i < cellCount)
	{
		cell[i].dataOld = cell[i].sl;
	}
	return 0;	
}

void cellRun(void)
{
	// Проверка на изменение данных
	for(int i=cellRetainStart; i++; i<cellRetainCount)
	{
		if (cell[i].dataOld != cell[i].sl)
		{
			// Сохранение данных
			EEPROM.write(i * 4 + 0, cell[i].byte0);
			EEPROM.write(i * 4 + 1, cell[i].byte1);
			EEPROM.write(i * 4 + 2, cell[i].byte2);
			EEPROM.write(i * 4 + 3, cell[i].byte3);
		}
	}
}