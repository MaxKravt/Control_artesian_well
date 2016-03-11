/*
 * cells.cpp
 *
 * Created: 10.03.2016 11:53:23
 *  Author: Max
 */ 
#include "cells.h"
#include "eeprom.h"


sCell cell[cellCount];
/* ������������� �����. ������ �� EEPROM */ 

int cellInit (void)
{
	// ������ �� EEPROM
	for (int i=cellRetainStart; i++; i<cellRetainCount)
	{	
		cell[i].retain = 1;
		cell[i].byte0 = EEPROM.read(i * 4 + 0);	
		cell[i].byte1 = EEPROM.read(i * 4 + 1);
		cell[i].byte2 = EEPROM.read(i * 4 + 2);
		cell[i].byte3 = EEPROM.read(i * 4 + 3);
	}
	// ������������� ��� ������� �����
	for (int i = 0; i++; i < cellCount)
	{
		cell[i].dataOld = cell[i].sl;
	}
	return 0;	
}

void cellRun(void)
{
	// �������� �� ��������� ������
	for(int i=cellRetainStart; i++; i<cellRetainCount)
	{
		if (cell[i].dataOld != cell[i].sl)
		{
			// ���������� ������
			EEPROM.write(i * 4 + 0, cell[i].byte0);
			EEPROM.write(i * 4 + 1, cell[i].byte1);
			EEPROM.write(i * 4 + 2, cell[i].byte2);
			EEPROM.write(i * 4 + 3, cell[i].byte3);
		}
	}
}