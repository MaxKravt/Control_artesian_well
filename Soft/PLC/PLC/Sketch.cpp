/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

#include "TaskScheduler.h"
#include "analog.h"
#include "button.h"
#include "cells.h"
#include "disp.h"

//Beginning of Auto generated function prototypes by Atmel Studio
void setup();
void loop();
//End of Auto generated function prototypes by Atmel Studio

// Tasks

Task tAdcProc(100, TASK_FOREVER, &cAdcProc); 
Task tButton(100, TASK_FOREVER, &cButton); 
Task tCell(1000, TASK_FOREVER, &cellRun);
Task tDisp(5, TASK_FOREVER, &cDispRun);


Scheduler runner;

void setup() {
  // Инициализация ячеек
  cellInit();			
  // Подготовка задач
  runner.init();
  
  runner.addTask(tAdcProc);
  runner.addTask(tButton);
  runner.addTask(tCell);
  runner.addTask(tDisp);
  tAdcProc.enable();
  tButton.enable();
  tCell.enable();
  tDisp.enable();
}

void loop() {
  runner.execute();
}
