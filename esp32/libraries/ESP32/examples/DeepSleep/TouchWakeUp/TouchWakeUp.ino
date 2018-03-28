/*
Deep Sleep with Touch Wake Up
=====================================
This code displays how to use deep sleep with
a touch as a wake up source and how to store data in
RTC memory to use it over reboots

This code is under Public Domain License.

Author:
Pranav Cherukupalli <cherukupallip@gmail.com>
*/
#include <Fluotube.h>

#define Threshold 40 /* Greater the value, more the sensitivity */

RTC_DATA_ATTR int bootCount = 0;
touch_pad_t touchPin;
/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case 1  : FluoTube.debugln("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : FluoTube.debugln("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : FluoTube.debugln("Wakeup caused by timer"); break;
    case 4  : FluoTube.debugln("Wakeup caused by touchpad"); break;
    case 5  : FluoTube.debugln("Wakeup caused by ULP program"); break;
    default : FluoTube.debugln("Wakeup was not caused by deep sleep"); break;
  }
}

/*
Method to print the touchpad by which ESP32
has been awaken from sleep
*/
void print_wakeup_touchpad(){
  touch_pad_t pin;

  touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch(touchPin)
  {

    case 7  : FluoTube.debugln("Touch detected on GPIO 27"); break;
    default : FluoTube.debugln("Wakeup not by touchpad"); break;
  }
}

void callback(){
  //placeholder callback function
}

void setup(){

  FluoTube.setup();

  delay(1000); //Take some time to open up the Serial Monitor

  //Increment boot number and print it every reboot
  ++bootCount;
  FluoTube.debugln("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32 and touchpad too
  print_wakeup_reason();
  print_wakeup_touchpad();

  //Setup interrupt on Touch BUTTON_CHECK (GPIO27), pay attention: only this GPIO is available !
  touchAttachInterrupt(BUTTON_CHECK, callback, Threshold);

  //Configure Touchpad as wakeup source
  esp_sleep_enable_touchpad_wakeup();

  //Go to sleep now
  FluoTube.debugln("Going to sleep now");
  esp_deep_sleep_start();
  FluoTube.debugln("This will never be printed");
}

void loop(){
  //This will never be reached
}
