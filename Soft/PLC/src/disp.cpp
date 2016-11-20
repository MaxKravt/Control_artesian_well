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
	{'B', 0b00011111},
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


void mDataUp(uint8_t in)     {cellAna[in].var++;}
void mDataDwn(uint8_t in)    {cellAna[in].var--;}
void menuUp(uint8_t in)      {_MENU_STEP = in;}
void menuDwn(uint8_t in)     {_MENU_STEP = in;}
void menuRight(uint8_t in)   {_MENU_STEP = in;}
void switchAvtReg(uint8_t in){_SAU_WORK = !_SAU_WORK;}


struct sMenuConf {
	void (* pFuncUp)(uint8_t) ;
	void (* pFuncDwn)(uint8_t) ;
	void (* pFuncRight)(uint8_t) ;
	uint8_t dataUp;
	uint8_t dataDwn;
	uint8_t dataRight;
	uint8_t numPoint;
	const __FlashStringHelper * menuS ;
	int16_t *data;
};
const char menuStringPGM[][7]  PROGMEM = {
	"0 P-", //0,0	Давлене воды
	"1 U-", //1,0	Уставка давления воды
	"2dU-", //2,0	Гистерезис уставки
	"3 I-", //3,0	Ток на упрвление насосом
	"4U1-", //4,0	Ток холостого хода
	"5U2-", //5,0	Ток максимальный
	"6U3-", //6,0	Максимально время включения насоса
	"7U4-", //7,0	Мин код АЦП
	"8U5-", //8,0	Макс код АЦП
	"9U6-", //9,0	Мин значение датчика
	"AU7-", //10,0	Макс значение датчика
	"B51-", //11,0	Количество включений
	"C52-",  //12,0	Время работы насоса
	" ",    //13
	" ",    //14
	" ",    //15
	" ",    //16
	" ",    //17
	" ",    //18
	" ",    //19
	"- U-", //20	Уставка давления воды
	"-dU-", //21  Гистерезис уставки
	"-U1-", //22  Ток холостого хода
	"-U2-", //23  Ток максимальный
	"-U3-", //24  Максимально время включения насоса
	"-U4-", //25  Мин код АЦП
	"-U5-", //26  Макс код АЦП
	"-U6-", //27  Мин значение датчика
	"-U7-", //28  Макс значение датчика
	"-51-", //29  Количество включений
	"-52-", //30  Время работы насоса
	" ",    //31
	" ",    //32
	" ",    //33
};


#define MENU_STR(x) (__FlashStringHelper *)&menuStringPGM[x][0]
const struct sMenuConf menuConf[]  PROGMEM = {
	{	menuUp, menuDwn, switchAvtReg,12,  1, 1,  1, MENU_STR(0), &_PRESS_IN},				//00
	{	menuUp, menuDwn, menuRight,    0,  2, 20, 1, MENU_STR(1), &_SET_P_VAL},				//01
	{	menuUp, menuDwn, menuRight,    1,  3, 21, 1, MENU_STR(2), &_DZ_P_VAL},				//02
	{	menuUp, menuDwn, switchAvtReg, 2,  4, 1,  1, MENU_STR(3), &_CURRENT_IN},			//03
	{	menuUp, menuDwn, menuRight,    3,  5, 22, 1, MENU_STR(4), &cellAna[26].var},	//04 Ток холостого хода
	{	menuUp, menuDwn, menuRight,    4,  6, 23, 1, MENU_STR(5), &cellAna[27].var},	//05 Ток максимальный
	{	menuUp, menuDwn, menuRight,    5,  7, 24, 0, MENU_STR(6), &cellAna[28].var},	//06 Максимально время включения насоса
	{	menuUp, menuDwn, menuRight,    6,  8, 25, 0, MENU_STR(7), &_ADC_MIN_A0},			//07 Мин код АЦП
	{	menuUp, menuDwn, menuRight,    7,  9, 26, 0, MENU_STR(8), &_ADC_MAX_A0},			//08 Макс код АЦП
	{	menuUp, menuDwn, menuRight,    8, 10, 27, 1, MENU_STR(9), &_VAL_MIN_A0},			//09 Мин значение датчика
	{	menuUp, menuDwn, menuRight,    9, 11, 28, 1, MENU_STR(10), &_VAL_MAX_A0},			//10 Макс значение датчика
	{	menuUp, menuDwn, menuRight,   10, 12, 29, 0, MENU_STR(11), &cellAna[29].var},	//11 Количество включений
	{	menuUp, menuDwn, menuRight,   11,  0, 30, 0, MENU_STR(12), &cellAna[30].var},	//12 Время работы насоса
                                                                                	// Резервы
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//13
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//14
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//15
																																								// Ошибки САУ
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//16
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//17
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//18
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//19

	{	mDataUp, mDataDwn, menuRight,  24, 24,  1, 1, MENU_STR(20), &_SET_P_VAL},	    //20
	{	mDataUp, mDataDwn, menuRight,  25, 25,  2, 1, MENU_STR(21), &_DZ_P_VAL},	    //21
	{	mDataUp, mDataDwn, menuRight,  26, 26,  4, 1, MENU_STR(22), &cellAna[26].var},//22 Ток холостого хода
	{	mDataUp, mDataDwn, menuRight,  27, 27,  5, 1, MENU_STR(23), &cellAna[27].var},//23 Ток максимальный
	{	mDataUp, mDataDwn, menuRight,  28, 28,  6, 0, MENU_STR(24), &cellAna[28].var},//24 Максимально время включения насоса
	{	mDataUp, mDataDwn, menuRight,  20, 20,  7, 0, MENU_STR(25), &_ADC_MIN_A0},	  //25 Мин код АЦП
	{	mDataUp, mDataDwn, menuRight,  21, 21,  8, 0, MENU_STR(26), &_ADC_MAX_A0},	  //26 Макс код АЦП
	{	mDataUp, mDataDwn, menuRight,  22, 22,  9, 1, MENU_STR(27), &_VAL_MIN_A0},	  //27 Мин значение датчика
	{	mDataUp, mDataDwn, menuRight,  23, 23, 10, 1, MENU_STR(28), &_VAL_MAX_A0},	  //28 Макс значение датчика
	{	mDataUp, mDataDwn, menuRight,  29, 29, 11, 0, MENU_STR(29), &cellAna[29].var},//29 Количество включений
	{	mDataUp, mDataDwn, menuRight,  30, 30, 12, 0, MENU_STR(30), &cellAna[30].var},//30 Время работы насоса
																																								  // Резервы
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//31
	{	menuUp, menuDwn, menuRight,   11,  0, 29, 1, MENU_STR(12), &cellAna[30].var},	//32
};


void menuControl(void){
	static uint32_t timeLastClick = 0;
	// Загрузка указателей на функции обработки
	void (* pFuncUp)(uint8_t)    = (void (*)(uint8_t)) pgm_read_ptr(&menuConf[_MENU_STEP].pFuncUp);
	void (* pFuncDwn)(uint8_t)   = (void (*)(uint8_t)) pgm_read_ptr(&menuConf[_MENU_STEP].pFuncDwn);
	void (* pFuncRight)(uint8_t) = (void (*)(uint8_t)) pgm_read_ptr(&menuConf[_MENU_STEP].pFuncRight);
	// Вывод данных на дисплей
	ledWriteString( String((__FlashStringHelper *)pgm_read_ptr(&menuConf[_MENU_STEP].menuS))
								+ String((float)(*((uint16_t *)pgm_read_ptr(&menuConf[_MENU_STEP].data)))
								/ (pow(10.0 ,(double)pgm_read_byte(&menuConf[_MENU_STEP].numPoint))),
								 pgm_read_byte(&menuConf[_MENU_STEP].numPoint)));
	// Обработка кнопок
	if(!_BUTTON_UP.var && _BUTTON_UP.var_o)     {
		pFuncUp   (pgm_read_byte(&menuConf[_MENU_STEP].dataUp));
		timeLastClick = millis();
	}
	if(!_BUTTON_DWN.var && _BUTTON_DWN.var_o)   {
		pFuncDwn  (pgm_read_byte(&menuConf[_MENU_STEP].dataDwn));
		timeLastClick = millis();
	}
	if(!_BUTTON_RGHT.var && _BUTTON_RGHT.var_o) {
		pFuncRight(pgm_read_byte(&menuConf[_MENU_STEP].dataRight));
		timeLastClick = millis();
		}
  // Сохранение прошлых значений
	_BUTTON_UP.var_o   = _BUTTON_UP.var;
	_BUTTON_DWN.var_o  = _BUTTON_DWN.var;
	_BUTTON_RGHT.var_o = _BUTTON_RGHT.var;
  // Если не было нажатий на кнопки 30 сек, то возвращаемся на вывод строки
	// давления. Часть работает кроме вывода Давления или Тока.
	if (!(_MENU_STEP == 0 || _MENU_STEP == 3)) {
		if (millis() - timeLastClick > 30000 ) {
			_MENU_STEP = 0;
		}
	}

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
