/*
      Под управлением САУ находится 5 реле. Для предотвращения выхода из строя
 * оборудования необходимо переключать их с соблюдением условия, что цепь
 * должна быть разорвана. Для лучшего понимания как надо делать напишу таблицу
 * переходов.
 * R1 -
 * R2 -
 * R3 -
 * R4 -
 * R5 -
 * ___________________________________________________________________
 * |                               |         Реле           |  Заде  |
 * |   Состояние                   +------------------------+  ржка  +
 * |                               | R1 | R2 | R3 | R4 | R5 |        |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 0 Отключено питание           | 0  | 0  | 0  | 0  | 0  |        |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 1 Инициализация               |    |    |    |    |    | 5 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 2 Работа плавного пуска (ППИО)|    |    |    |    |    | 0 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 3 Переключение на САУ         |    |    |    |    |    | 10 сек |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 4 Переключение на ППИО        |    |    |    |    |    | 10 сек |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 5 Насос отключен              |    |    |    |    |    | 0 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 6 Включение насоса.  Шаг 1    |    |    |    |    |    | 1 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 7 Включение насоса.  Шаг 2    |    |    |    |    |    | 0 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 8 Насос Включен               |    |    |    |    |    | 0 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 9 Отключение насоса. Шаг 1    |    |    |    |    |    | 5 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 10 Отключение насоса. Шаг 2   |    |    |    |    |    | 0 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 * | 20 Любая ошибка               |    |    |    |    |    | 0 сек  |
 * +-------------------------------+----+----+----+----+----+--------+
 */


#include "algoritm.h"
#include "arduino.h"
#include "analog.h"
#include "define.h"
#include "cells.h"

void calcCurrent(void);


// Этапы переключения САУ
#define _STEP_01_INIT            0
#define _STEP_02_PPIO_WORK       1
#define _STEP_03_TO_SAU          2
#define _STEP_04_TO_PPIO         3
#define _STEP_05_PUMP_STOP       4
#define _STEP_06_PUMP_GO_1       5
#define _STEP_07_PUMP_GO_2       6
#define _STEP_08_PUMP_WORK       7
#define _STEP_09_PUMP_STOP_01    8
#define _STEP_10_PUMP_STOP_02    9
#define _STEP_ERROR              20


const struct sReleyConf{
  uint16_t nom:8;
  uint16_t R1:1;
  uint16_t R2:1;
  uint16_t R3:1;
  uint16_t R4:1;
  uint16_t R5:1;
  uint16_t time;
} releyConf[] = {
  //nom,R1,R2,R3,R4,R5,time
  {   0, 0, 0, 0, 0, 0, 5000},
  {   1, 0, 0, 0, 0, 0, 0},
  {   2, 0, 0, 0, 0, 0, 10000},
  {   3, 0, 0, 0, 0, 0, 10000},
  {   4, 0, 0, 0, 0, 0, 0},
  {   5, 0, 0, 0, 0, 0, 1000},
  {   6, 0, 0, 0, 0, 0, 0},
  {   7, 0, 0, 0, 0, 0, 0},
  {   8, 0, 0, 0, 0, 0, 5000},
  {   9, 0, 0, 0, 0, 0, 0},
  {  10, 0, 0, 0, 0, 0, 0},
};

void setReleyConf(const struct sReleyConf *in){
  cellBool[0].var = in->R1;
  cellBool[1].var = in->R2;
  cellBool[2].var = in->R3;
  cellBool[3].var = in->R4;
  cellBool[4].var = in->R5;
}

void cAlgoritm(){
  static sTimerOn tonStep[10];
  static uint8_t fst_cycl = 1;
  if (fst_cycl == 1) {
    fst_cycl = 0;
    _ALG_STEP.var = _STEP_01_INIT;
    for (size_t i = 0; i < 10; i++) {
      tonStep[i].in = 0;
      tonStep[i].time = (uint32_t) releyConf[i].time;
    }
  }
  for (size_t i = 0; i < 10; i++) {
    timeON(&tonStep[i]);
  }

  switch (_ALG_STEP.var)  {
    case _STEP_01_INIT:{
      tonStep[0].in = 1;
      setReleyConf(&releyConf[0]);
      if ((tonStep[0].out) == 1) {
        tonStep[0].in = 0;
        _ALG_STEP.var = _STEP_02_PPIO_WORK;
      }
      break;
    }
    case _STEP_02_PPIO_WORK:{
      setReleyConf(&releyConf[1]);
      if (_SAU_WORK ) {
        _ALG_STEP.var = _STEP_03_TO_SAU;
      }
      break;
    }
    case _STEP_03_TO_SAU:{
      setReleyConf(&releyConf[2]);
      tonStep[2].in = 1;
      if (tonStep[2].out && _SAU_WORK ) {
        tonStep[2].in = 0;
        _ALG_STEP.var = _STEP_05_PUMP_STOP;
      }
      if (!_SAU_WORK ) {
        tonStep[2].in = 0;
        _ALG_STEP.var = _STEP_04_TO_PPIO;
      }
      break;
    }
    case _STEP_04_TO_PPIO:{
      setReleyConf(&releyConf[3]);
      tonStep[3].in = 1;
      if (_SAU_WORK ) {
        tonStep[3].in = 0;
        _ALG_STEP.var = _STEP_03_TO_SAU;
      }
      if (tonStep[3].out && !_SAU_WORK ) {
        tonStep[3].in = 0;
        _ALG_STEP.var = _STEP_02_PPIO_WORK;
      }
      break;
    }
    case _STEP_05_PUMP_STOP:{
      setReleyConf(&releyConf[4]);
      tonStep[4].in = _SET_P_LO.var; /*низкое давление*/
      if (!_SAU_WORK ) {
        tonStep[4].in = 0;
        _ALG_STEP.var = _STEP_04_TO_PPIO;
      }
      if (tonStep[4].out) {
        tonStep[4].in = 0;
        _ALG_STEP.var = _STEP_06_PUMP_GO_1;
      }
      break;
    }
    case _STEP_06_PUMP_GO_1:{
      setReleyConf(&releyConf[5]);
      tonStep[5].in = 1;

      if (!_SAU_WORK ) {
        tonStep[5].in = 0;
        _ALG_STEP.var = _STEP_04_TO_PPIO;
      }

      if (tonStep[5].out) {
        tonStep[5].in = 0;
        _ALG_STEP.var = _STEP_07_PUMP_GO_2;
      }
      break;
    }
    case _STEP_07_PUMP_GO_2:{
      setReleyConf(&releyConf[6]);
      tonStep[6].in = 1;
      if (!_SAU_WORK ) {
        tonStep[6].in = 0;
        _ALG_STEP.var = _STEP_04_TO_PPIO;
      }
      if (tonStep[6].out) {
        tonStep[6].in = 0;
        _ALG_STEP.var = _STEP_08_PUMP_WORK;
      }
      break;
    }
    case _STEP_08_PUMP_WORK:{
      setReleyConf(&releyConf[7]);
      _PUMP_WORK = 1;
      tonStep[7].in = _SET_P_HI.var;
      if (!_SAU_WORK ) {
        tonStep[7].in = 0;
        _PUMP_WORK = 0;
        _ALG_STEP.var = _STEP_04_TO_PPIO;
      }
      if (tonStep[7].out) {
        tonStep[7].in = 0;
        _PUMP_WORK = 0;
        _ALG_STEP.var = _STEP_09_PUMP_STOP_01;
      }
      break;
    }
    case _STEP_09_PUMP_STOP_01:{
      setReleyConf(&releyConf[8]);
      tonStep[8].in = 1;
      if (!_SAU_WORK ) {
        tonStep[8].in = 0;
        _ALG_STEP.var = _STEP_04_TO_PPIO;
      }
      if (tonStep[8].out) {
        tonStep[8].in = 0;
        _ALG_STEP.var = _STEP_10_PUMP_STOP_02;
      }
      break;
    }
    case _STEP_10_PUMP_STOP_02:{
      setReleyConf(&releyConf[9]);
      tonStep[9].in = 1;
      if (!_SAU_WORK ) {
        tonStep[9].in = 0;
        _ALG_STEP.var = _STEP_04_TO_PPIO;
      }
      if (tonStep[9].out) {
        tonStep[9].in = 0;
        _ALG_STEP.var = _STEP_05_PUMP_STOP;
      }
      break;
    }
    case _STEP_ERROR:
      break;
  };

  // Вычисление тока насоса
  calcCurrent();

  // вычисление Времени работы насоса
  static uint32_t summTimePumpWork = 0;
  static uint32_t timePumpWork = 0;
  if(_ALG_STEP.var == _STEP_08_PUMP_WORK && _ALG_STEP.var_o != _STEP_08_PUMP_WORK){
    timePumpWork = millis();
    // вычисление количества включений
    cellAna[29].var += 1;
  }
  if(_ALG_STEP.var != _STEP_08_PUMP_WORK && _ALG_STEP.var_o == _STEP_08_PUMP_WORK)
    summTimePumpWork += millis() - timePumpWork;
  if(summTimePumpWork > 60000){
      cellAna[30].var += 60000;
      summTimePumpWork -= 60000;
    }
  _ALG_STEP.var_o = _ALG_STEP.var;

  // вычисление недостоверности датчика давления
  cellBool[14].var = _PRESS_ADC_IN < 2 || _PRESS_ADC_IN > 1020 ? 1 : 0;

  // вычисление недостоверности датчика тока
  cellBool[15].var = _CURRENT_ADC_IN < 2 || _CURRENT_ADC_IN > 1020 ? 1 : 0;

  // Вычисление устовок по давлению
  _SET_P_HI.var = _PRESS_IN > (_SET_P_VAL + _DZ_P_VAL )? 1 :0;

  // Вычисление уставок по току
  _SET_P_LO.var = _PRESS_IN < (_SET_P_VAL - _DZ_P_VAL )? 1 :0;

  // Передача управления на физический выход
  digitalWrite(_DIS_RELE_K1, cellBool[0].var);
  digitalWrite(_DIS_RELE_K2, cellBool[1].var);
  digitalWrite(_DIS_RELE_K3, cellBool[2].var);
  digitalWrite(_DIS_RELE_K4, cellBool[3].var);
  digitalWrite(_DIS_RELE_K5, cellBool[4].var);
  digitalWrite(_DIS_LED_SAU, _SAU_WORK);
  digitalWrite(_DIS_LED_WORK,_PUMP_WORK);

}

void calcCurrent(void){
  static uint16_t currentArrayMed[COUNT_CURRENT_ARRAY_MED];
  static uint16_t pointCurrentArrayMed=0;
  // Расчет тока
  if (cellBool[20].var == 1){
    float tempSumm = 0.0, tempCurrent = 0.0;
    for (size_t i = 0; i < COUNT_CURRENT_ARRAY; i++){
      tempSumm += (float)currentArray[i];
    }
    float tempAver = tempSumm / COUNT_CURRENT_ARRAY;
    uint16_t tempAverInt = (uint16_t) tempAver;
    for (size_t i = 0; i < COUNT_CURRENT_ARRAY; i++){
      tempCurrent += tempAverInt > currentArray[i]
                    ? tempAver - (float)currentArray[i]
                    : (float)currentArray[i] -tempAver;
    }
    //_CURRENT_IN = (uint16_t)tempCurrent / COUNT_CURRENT_ARRAY;
    cellBool[20].var = 0;

    pointCurrentArrayMed = pointCurrentArrayMed < (COUNT_CURRENT_ARRAY_MED - 1)
                        ? pointCurrentArrayMed + 1
                        :0;
    currentArrayMed[pointCurrentArrayMed] =
        (uint16_t)tempCurrent / COUNT_CURRENT_ARRAY;
    uint16_t arrayCurrentValMax = currentArrayMed[0];
    uint16_t arrayCurrentValMin = currentArrayMed[0];
    size_t arrayCurrentIndxMedMax = 0;
    size_t arrayCurrentIndxMedMin = 0;

    for (size_t i = 1; i < COUNT_CURRENT_ARRAY_MED; i++) {
      if (currentArrayMed[i] > arrayCurrentValMax) {
        arrayCurrentValMax = currentArrayMed[i];
        arrayCurrentIndxMedMax = i;
      }
      if (currentArrayMed[i] < arrayCurrentValMin) {
        arrayCurrentValMin = currentArrayMed[i];
        arrayCurrentIndxMedMin = i;
      }
    }
    uint16_t tempSummMed = 0;
    uint16_t tempSummCountMed = 0;
    for (size_t i = 0; i < COUNT_CURRENT_ARRAY_MED; i++) {
      if(i != arrayCurrentIndxMedMax && i != arrayCurrentIndxMedMin){
        tempSummMed += currentArrayMed[i];
        tempSummCountMed ++;
      }
    }
    uint16_t tempMed = tempSummMed / tempSummCountMed;
    _CURRENT_IN = tempMed;
  }
}

void timeON(sTimerOn * data){
  if(data->in == 0){
    data->acc = millis();
    data->out = 0;
    data->run = 0;
  }
  else{
    if ((millis() - data->acc) >= data->time) {
      data->out = 1;
      data->run = 0;
      data->acc = millis() + data->time + 1000;
    }
    else{
      data->run = 1;
      data->out = 0;
    }
  }
}
