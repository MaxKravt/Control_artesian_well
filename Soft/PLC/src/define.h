

#ifndef DEFINE_H_
#define DEFINE_H_
#include <Arduino.h>

#define _ANA_PRESS_PUMP       A0
#define _ANA_CURRENT_PUPM     A4



#define _DIS_RELE_K1         2
#define _DIS_RELE_K2         3
#define _DIS_RELE_K3         4
#define _DIS_RELE_K4         5
#define _DIS_RELE_K5         7


#define _DIS_LED_SAU         8
#define _DIS_LED_WORK        9

#define _DIS_BUTTTON_1       A1
#define _DIS_BUTTTON_2       A2
#define _DIS_BUTTTON_3       A3

// Описание аналоговых ячеек
#define _PRESS_ADC_IN      cellAna[0].var   // 00 - значение кода АЦП канала  А0
#define _PRESS_IN          cellAna[1].var   // 01 - физическое значение давления воды
#define _TIME_POW_ON       cellAna[2].var   // 02 - время включения насоса
#define _CURRENT_ADC_IN    cellAna[3].var   // 03 - показания датчика тока
#define _CURRENT_IN        cellAna[4].var   // 04 - Физическое значение тока
// 05
// 06
// 07
// 08
// 09
#define _ALG_STEP      cellAna[10] // 10 - шаг алгоритма
#define _DISP_STEP     cellAna[11].var // 11 - шаг в автомате дисплея
#define _MENU_STEP     cellAna[12].var // 12 - шаг в меню
// 13
// 14
// 15
// 16
// 17
// 18
// 19
#define _ADC_MIN_A0   cellAna[20].var  // 20 - минимальное значение кода АЦП при 0 давлении
#define _ADC_MAX_A0   cellAna[21].var  // 21 - максимальное значение кода АЦП при максимальном давлении
#define _VAL_MIN_A0   cellAna[22].var  // 22 - минимальное давление
#define _VAL_MAX_A0   cellAna[23].var  // 23 - максимальное давлеие
#define _SET_P_VAL    cellAna[24].var  // 24 - уставка давления
#define _DZ_P_VAL     cellAna[25].var  // 25 - зона нечувствительности уставки давлния.
// 26 - ток холостого хода
// 27 - Максимальный ток
// 28 - Максимальное время включения
// 29 - Количество включений
// 30 - Время работы насоса

// Описание дискретных ячеек
// 00 - Команда на включение реле реле К1
// 01 - Команда на включение реле реле К2
// 02 - Команда на включение реле реле К3
// 03 - Команда на включение реле реле К4
// 04 - Команда на включение реле реле К5
#define _SAU_WORK      cellBool[ 5].var  // 05 - Работа автоматики САУ - 1, Работа автоматики ППИО системы -0
#define _PUMP_WORK     cellBool[ 6].var  // 06 - Индикатор работы насоса
#define _BUTTON_UP     cellBool[ 7] // 07 - Нажата кнопка вверх
#define _BUTTON_DWN    cellBool[ 8] // 08 - Нажата кнопка вниз
#define _BUTTON_RGHT   cellBool[ 9] // 09 - Нажата кнопка применить
// 10 -
// 11 -
// 12 -
// 13 -
// 14 - Недомтоверность показаний датчика давления
// 15 - Недостоверность показаний датчика тока
// 16 - Превышено время работы насоса
// 17 -
#define _SET_P_LO         cellBool[18]// 18 - давление воды низкое
#define _SET_P_HI         cellBool[19]// 19 - давление воды высокое
// 20 - Готовность данных для расчета тока
// 21 -
// 00 -
// 00 -
// 00 -
// 00 -
// 00 -
// 00 -
// 00 -
// 00 -
// 00 -
#endif /* DEFINE_H_ */
