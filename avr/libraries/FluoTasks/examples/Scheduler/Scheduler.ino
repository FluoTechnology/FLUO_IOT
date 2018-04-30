#include <FluoTasks.h>

FluoTasks myTask;

void setup() {
  Serial.begin(115200);
  Serial.println("AVR booting ...");

  myTask.begin();

  myTask.addTask(loop_1, myTask.convertMs(1000));
  myTask.addTask(loop_2, myTask.convertMs(5000));

  delay(1000);

  pinMode(13, OUTPUT);

}

void loop_1() {
  digitalWrite(13, HIGH);
  Serial.print(".");
}

void loop_2() {
  digitalWrite(13, LOW);
  Serial.print("-");
}


void loop() {

}