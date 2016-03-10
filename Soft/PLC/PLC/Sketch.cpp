/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

#include "TaskScheduler.h"
#include "analog.h"
#include "button.h"
//Beginning of Auto generated function prototypes by Atmel Studio
void setup();
void loop();
//End of Auto generated function prototypes by Atmel Studio

// Tasks

Task tAdcProc(500, TASK_FOREVER, &cAdcProc); 
Task tButton(100, TASK_FOREVER, &cButton); 



Scheduler runner;

void setup() {
  runner.init();
  runner.addTask(tAdcProc);
  runner.addTask(tButton);
  tAdcProc.enable();
  tButton.enable();
  
  cellInit();
}

void loop() {
  runner.execute();
}
