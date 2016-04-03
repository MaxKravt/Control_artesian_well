
#include <Arduino.h>
#include "TaskScheduler.h"
#include "analog.h"
#include "button.h"
#include "cells.h"
#include "disp.h"
#include "mbPort.h"


void setup();
void loop();

// Tasks

Task tAdcProc(100, TASK_FOREVER, &cAdcProc);
Task tButton(100, TASK_FOREVER, &cButton);
Task tCell(1000, TASK_FOREVER, &cellRun);
Task tDisp(5, TASK_FOREVER, &cDispRun);
Task tPort(1, TASK_FOREVER, &port2Task);

Scheduler runner;

void setup() {
  // Инициализация ячеек
  cellInit();
  // Подготовка задач
  port2BeginTask();
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial2.print("test");
  pinMode(LED_BUILTIN, OUTPUT);


  runner.init();
  runner.addTask(tAdcProc);
  runner.addTask(tButton);
  runner.addTask(tCell);
  runner.addTask(tDisp);
  runner.addTask(tPort);
  tAdcProc.enable();
  tButton.enable();
  tCell.enable();
  tDisp.enable();
  tPort.enable();
}

void loop() {
  runner.execute();
}
