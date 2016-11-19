#define _TASK_TIMECRITICAL

#include <Arduino.h>
#include <TaskScheduler.h>
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
//Task tButton(100, TASK_FOREVER, &cButton);
Task tCell    (2000, TASK_FOREVER, &cellRun);
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
  cellBool[0].var = true;
  cellBool[1].var = true;
  cellBool[2].var = true;
  cellBool[3].var = true;
  cellBool[4].var = true;

  digitalWrite(_DIS_RELE_K1, 1);
  digitalWrite(_DIS_RELE_K2, 1);
  digitalWrite(_DIS_RELE_K3, 1);
  digitalWrite(_DIS_RELE_K4, 1);

  pinMode(_DIS_RELE_K1, OUTPUT);
  pinMode(_DIS_RELE_K2, OUTPUT);
  pinMode(_DIS_RELE_K3, OUTPUT);
  pinMode(_DIS_RELE_K4, OUTPUT);
  pinMode(_DIS_RELE_K5, OUTPUT);

  runner.init();
  runner.addTask(tAdcProc);
  //runner.addTask(tButton);
  runner.addTask(tCell);
  runner.addTask(tDisp);
  runner.addTask(tPort);
  runner.addTask(tAlgoritm);

  tAdcProc.enable();
  //tButton.enable();
  tCell.enable();
  tDisp.enable();
  tPort.enable();
  tAlgoritm.enable();
}

void loop() {
  cellAna[18].var =  tAdcProc.getOverrun();
  cellAna[19].var =  tAdcProc.getStartDelay();
  runner.execute();
}
