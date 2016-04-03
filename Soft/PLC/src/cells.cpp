/*
 * cells.cpp
 *
 * Created: 10.03.2016 11:53:23
 *  Author: Max
 */
#include "cells.h"
#include "eeprom.h"


sCellAna cellAna[cellCount];
sCellBool cellBool[cellCount];
/* Инициализация ячеек. Чтение из EEPROM */

int cellInit (void)
{
	// Чтение из EEPROM
	for (uint16_t i=cellRetainStart; i < cellRetainStart + cellRetainCount; i++)
	{
		cellAna[i].retain = 1;
		cellAna[i].byte0 = EEPROM.read(i * 4 + 0);
		cellAna[i].byte1 = EEPROM.read(i * 4 + 1);
		cellAna[i].byte2 = EEPROM.read(i * 4 + 2);
		cellAna[i].byte3 = EEPROM.read(i * 4 + 3);
	}
	// инициализация для первого цикла
	for (uint16_t i = 0; i < cellCount; i++)
	{
		cellAna[i].var_o = cellAna[i].var;
	}
	return 0;
}

void cellRun(void)
{
	// Проверка на изменение данных
	for(uint16_t i=cellRetainStart; i < cellRetainStart + cellRetainCount; i++)
	{
		if (cellAna[i].var_o != cellAna[i].var)
		{
			// Сохранение данных
			EEPROM.write(i * 4 + 0, cellAna[i].byte0);
			EEPROM.write(i * 4 + 1, cellAna[i].byte1);
			EEPROM.write(i * 4 + 2, cellAna[i].byte2);
			EEPROM.write(i * 4 + 3, cellAna[i].byte3);
			cellAna[i].var_o = cellAna[i].var;
		}


	}
}

uint8_t Cell_b_get_var(uint16_t nom)
{
	if (nom < MAX_CELL_BOOL )
		return cellBool[nom].var;
	else
		return 0;
}

int16_t  Cell_a_get_var(uint16_t nom)
{
	if (nom < MAX_CELL_ANA )
		return cellAna[nom].var;
	else
		return 0;
}

int8_t Cell_b_set_var(int8_t var, uint16_t nom)
{
	if (nom < MAX_CELL_BOOL )
	{	 cellBool[nom].var = var;
		 return CELL_OK;
	}
	else
		return CELL_ERROR;
}

int8_t Cell_a_set_var(int16_t var, uint16_t nom)
{
	if (nom < MAX_CELL_ANA )
	{	 cellAna[nom].var = var;
		 return CELL_OK;
	}
	else
		return CELL_ERROR;
}

struct sCellBool * Cell_b_get_ptr(void)
{
	return cellBool;
}
struct sCellAna  * Cell_a_get_ptr(void)
{
	return cellAna;
}

int8_t Cell_b_get (struct sCellBool * pCell_b, uint16_t nom)
{
	if (nom <= MAX_CELL_BOOL && nom >= 0 )
	{
		pCell_b->var = cellBool[nom].var;
		pCell_b->lock = cellBool[nom].lock;
		return CELL_OK;
	}
	else
		return CELL_ERROR;
}

int8_t Cell_a_get (struct sCellAna * pCell_a, uint16_t nom)
{
	if (nom <= MAX_CELL_ANA && nom >= 0 )
	{
		pCell_a->var =   cellAna[nom].var;
		pCell_a->lock =  cellAna[nom].lock;
		return CELL_OK;
	}
	else
		return CELL_ERROR;
}

int8_t Cell_b_set (uint16_t set, uint16_t nom)
{
	if (nom <= MAX_CELL_BOOL && nom >= 0 )
	{
		if (cellBool[nom].lock)
			return CELL_LOCK;

		cellBool[nom].var = set;
		return CELL_OK;
	}

	return CELL_ERROR;
}
int8_t Cell_a_set (uint16_t set, uint16_t nom)
{
	if (nom <= MAX_CELL_ANA && nom >= 0 )
	{
		if (cellAna[nom].lock)
			return CELL_LOCK;

		cellAna[nom].var = set;
		return CELL_OK;
	}

	return CELL_ERROR;
}
// Запись в ячейку реала 2 регистра
int8_t Cell_r_set (float set, uint16_t nom)
{
	if (nom <= MAX_CELL_ANA && nom >= 0 )
	{
		if (cellAna[nom].lock)
			return CELL_LOCK;
		unsigned long temp;
		memcpy(&temp, &set , sizeof(float));
		cellAna[nom + 0].var = (int)(0xFFFFUL & temp);
		cellAna[nom + 1].var = (int)(0xFFFFUL & (temp>> 16));
		return CELL_OK;
	}
	return CELL_ERROR;
}
