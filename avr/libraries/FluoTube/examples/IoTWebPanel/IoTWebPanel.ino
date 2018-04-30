/*
  IoTWebPanel

  Sketch to AVR side !

  To available Fluo IoT Panel:
  http://fluotechnology.co.uk/home/wiki/#iot

  modified 8 Gen 2018
  by Adriano Costanzo

  This example code is in the public domain.

  http://www.fluotecnology.co.uk/
*/

#include <FluoTube.h>

void setup() {

  Serial.begin(115200);
  Serial.println("Fluo Start...");
  
  FluoTube.Begin();
}

void loop() {
  FluoTube.Run();
}

