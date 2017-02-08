#define _TASK_TIMECRITICAL

#include <Arduino.h>
#include "TaskScheduler.h"
#include "analog.h"
#include "button.h"
#include "cells.h"
#include "disp.h"
#include "mbPort.h"
#include "algoritm.h"
#include "define.h"


void setup();
void loop();

// Tasks

Task tAdcProc (2,    TASK_FOREVER, &cAdcProc);
Task tButton  (20, TASK_FOREVER, &cButton);
Task tCell    (10000, TASK_FOREVER, &cellRun);
Task tDisp    (100,  TASK_FOREVER, &cDispRun);
Task tPort    (1,    TASK_FOREVER, &port2Task);
Task tAlgoritm(100,  TASK_FOREVER, &cAlgoritm);

Scheduler runner;

void setup() {
  // Инициализация ячеек
  cellInit();
  // Подготовка задач
  port2BeginTask();
  Serial.begin(115200);

  // init digital
  digitalWrite(_DIS_RELE_K1, 1);
  digitalWrite(_DIS_RELE_K2, 1);
  digitalWrite(_DIS_RELE_K3, 1);
  digitalWrite(_DIS_RELE_K4, 1);
  digitalWrite(_DIS_RELE_K5, 0);
  digitalWrite(_DIS_LED_SAU, 1);
  digitalWrite(_DIS_LED_WORK, 1);

  pinMode(_DIS_RELE_K1, OUTPUT);
  pinMode(_DIS_RELE_K2, OUTPUT);
  pinMode(_DIS_RELE_K3, OUTPUT);
  pinMode(_DIS_RELE_K4, OUTPUT);
  pinMode(_DIS_RELE_K5, OUTPUT);
  pinMode(_DIS_LED_SAU, OUTPUT);
  pinMode(_DIS_LED_WORK, OUTPUT);
  analogWrite(_DIS_RELE_K5_PWM, 15);
  pinMode(_DIS_RELE_K5_PWM, OUTPUT);

  pinMode(_DIS_BUTTTON_1, INPUT_PULLUP);
  pinMode(_DIS_BUTTTON_2, INPUT_PULLUP);
  pinMode(_DIS_BUTTTON_3, INPUT_PULLUP);

  runner.init();
  runner.addTask(tAdcProc);
  runner.addTask(tButton);
  runner.addTask(tCell);
  runner.addTask(tDisp);
  runner.addTask(tPort);
  runner.addTask(tAlgoritm);


  tAdcProc.enable();
  tButton.enable();
  tCell.enable();
  tDisp.enable();
  tPort.enable();
  tAlgoritm.enable();
}

void loop() {
//  cellAna[18].var =  tDisp.getOverrun();
//  cellAna[19].var =  tDisp.getStartDelay();
  runner.execute();
}
