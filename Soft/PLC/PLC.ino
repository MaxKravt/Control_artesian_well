#include <TaskScheduler.h>
#include "analog.h"
#include "button.h"
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
}

void loop() {
  runner.execute();
}
