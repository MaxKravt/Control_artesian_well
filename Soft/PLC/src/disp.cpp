/*
 * disp.cpp
 *
 * Created: 11.03.2016 13:17:10
 *  Author: Max
 * Дисплей используется 7 сегментный, под управлением микросхемы MAX7221
 * Подключение:
 * VCC   - +5 V
 * GND   - GND
 * DATAI - D11
 * CS    - D10
 * CLS   - D13
 */

#include <spi.h>
#include <pins_arduino.h>
#include <stdio.h>
#include "disp.h"
#include "cells.h"
#include "define.h"

/*        A
 *			F   B
 *				G
 *			E	  C
 *				D     dP
 *
 *  7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 * dP | A | B | C | D | E | F | G |
 */
const struct sLedCharConfig  charDecod [] = {
	//      76543210
	{'0', 0b01111110},
	{'1', 0b00110000},
	{'2', 0b01101101},
	{'3', 0b01111001},
	{'4', 0b00110011},
	{'5', 0b01011011},
	{'6', 0b01011111},
	{'7', 0b01110000},
	{'8', 0b01111111},
	{'9', 0b01111011},
	{'-', 0b00000001},
	{' ', 0b00000000},
	{'A', 0b01110111},
	{'B', 0b01111111},
	{'C', 0b01001110},
	{'d', 0b00111101},
	{'E', 0b01001111},
	{'F', 0b01000111},
	{'H', 0b00110111},
	{'P', 0b01100111},
	{'.', 0b10000000},
	{'U', 0b00111110},
	{'I', 0b00000110}

};


const char menuStringPGM[][7]  PROGMEM = {
	"0  P-", //0,0	Давлене воды
	"1  U-", //0,0	Уставка давления воды
	"2 dU-", //0,0	Гистерезис уставки
	"3  I-", //0,0	Ток на упрвление насосом
	"4 U1-", //0,0	Ток холостого хода
	"5 U2-", //0,0	Ток максимальный
	"6 U3-", //0,0	Максимально время включения насоса
	"7 U4-", //0,0	Мин код АЦП
	"8 U5-", //0,0	Макс код АЦП
	"9 U6-", //0,0	Мин значение датчика
	"A U7-", //0,0	Макс значение датчика
	"B S1-", //0,0	Количество включений
	"C S2-" //0,0	Время работы насоса
};

const __FlashStringHelper * menuString[] = {
	(__FlashStringHelper *) &menuStringPGM[0][0],
	(__FlashStringHelper *) &menuStringPGM[1][0],
	(__FlashStringHelper *) &menuStringPGM[2][0],
	(__FlashStringHelper *) &menuStringPGM[3][0],
	(__FlashStringHelper *) &menuStringPGM[4][0],
	(__FlashStringHelper *) &menuStringPGM[5][0],
	(__FlashStringHelper *) &menuStringPGM[6][0],
	(__FlashStringHelper *) &menuStringPGM[7][0],
	(__FlashStringHelper *) &menuStringPGM[8][0],
	(__FlashStringHelper *) &menuStringPGM[9][0],
	(__FlashStringHelper *) &menuStringPGM[10][0],
	(__FlashStringHelper *) &menuStringPGM[11][0],
	(__FlashStringHelper *) &menuStringPGM[12][0]
};




struct sMenuConf {
	void (* pFuncUp)(uint8_t) ;
	void (* pFuncDwn)(uint8_t) ;
	void (* pFuncRight)(uint8_t) ;
	uint8_t dataUp;
	uint8_t dataDwn;
	uint8_t dataRight;
	const __FlashStringHelper * menuS ;
	int16_t *data;
};


void menuUp(uint8_t in){_MENU_STEP = in;}
void menuDwn(uint8_t in){_MENU_STEP = in;}
void menuRight(uint8_t in){_MENU_STEP = in;}
void switchAvtReg(uint8_t in){_SAU_WORK = !_SAU_WORK;}


const struct sMenuConf menuConf[]= {
	{	menuUp, menuDwn, switchAvtReg, 4,  1, 1, menuString[0], &cellAna[01].var},
	{	menuUp, menuDwn, switchAvtReg, 0,  2, 1, menuString[1], &cellAna[02].var	},
	{	menuUp, menuDwn, switchAvtReg, 1,  3, 1, menuString[2], &cellAna[03].var	},
	{	menuUp, menuDwn, menuRight,    2,  4, 1, menuString[3], &cellAna[04].var	},
	{	menuUp, menuDwn, menuRight,    3,  0, 1, menuString[4], &cellAna[05].var	}
};


void menuControl(void){
	ledWriteString( String(menuConf[_MENU_STEP].menuS)  + String((float)(*menuConf[_MENU_STEP].data)/10.0, 1));
}

/*
 * Функция цикличного выполнения. Выводит данные на дисплей
 */
void cDispRun(void){
	// Счетчик для различных циклов. Статический
	// static uint8_t i = 0;
	switch(_DISP_STEP) {
		case 0:{  // инициализация
  		ledInit();
			_DISP_STEP = 1;
			break;
		}
		case 1:{ // при включении выводитс¤ приветствие
			// выводитс¤ давлене воды
			//String str;
			//static float fl = 1.0;
			//sprintf(str, "0 P-%03i", i);
			//fl = fl + 0.03;
			//str  =  String("0 P-") +  String(fl, 1);
			//ledWriteString(str);
			menuControl();
			break;
		}
		default: // неисправный конечный автомат
			_DISP_STEP = 0;
	}
}

/*
 * Функция вывода строки на дисплей.
 * Функция учитывает точку в строке.
 */
void ledWriteString(String in){
	uint8_t i =0, j =0;
	do {
		if (in[i+1] == '.'){
				ledWriteChar(8-j, ledDecode(in[i]) + ledDecode(in[i+1]));
				j += 1;	i += 2;
		}
		else	{
			ledWriteChar(8-j, ledDecode(in[i]));
			j += 1;	i += 1;
		}
	}
	while(j <= 7);
}

/*
 * Функция декодирования символов в элементы дисплея.
 */
uint8_t ledDecode (uint8_t in){
   for( uint8_t i=0; i < sizeof (charDecod)/sizeof(charDecod[0]); i++){
		 if (in==charDecod[i].index) return charDecod[i].decod;
	 }
	 return 0;
}

/*
 * Функция инициализации дисплея.
 */
void ledInit (void){
	digitalWrite(10, HIGH);
	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);
	SPI.begin();
	ledWriteChar(0x0C, 0x01); // Включить дисплей
	ledWriteChar(0x0F, 0x00); // Режим теста
	ledWriteChar(0x0b, 0x07); // Количество символов
	ledWriteChar(0x0A, 0x00); // Яркость
	// Кодирование BCD
	// 0xFF - Кодирование BCD
	// 0x00 - без кодировки
	ledWriteChar(0x09, 0x00);
}
/*
 * Функция записи символа в укузанный эелемент дисплея.
 */
void ledWriteChar (uint8_t addr, uint8_t data ){
	digitalWrite(10, LOW);
	SPI.transfer(addr);
	SPI.transfer(data);
	//SPI.endTransaction();
	digitalWrite(10, HIGH);
}
