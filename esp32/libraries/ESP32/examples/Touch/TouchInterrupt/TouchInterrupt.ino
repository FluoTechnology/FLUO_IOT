/*
This is un example howto use Touch Intrrerupts
The bigger the threshold, the more sensible is the touch
*/
#include <Fluotube.h>

int threshold = 40;
bool touch1detected = false;

void gotTouch1(){
 touch1detected = true;
}

void setup() {

  FluoTube.setup();

  delay(1000); // give me time to bring up serial monitor

  FluoTube.debugln("ESP32 Touch Interrupt Test");

  //Setup interrupt on Touch BUTTON_CHECK (GPIO27), pay attention: only this GPIO is available !
  touchAttachInterrupt(BUTTON_CHECK, gotTouch1, threshold);
}

void loop(){
  if(touch1detected){
    touch1detected = false;
    FluoTube.debugln("Touch 1 detected");
  }
}
