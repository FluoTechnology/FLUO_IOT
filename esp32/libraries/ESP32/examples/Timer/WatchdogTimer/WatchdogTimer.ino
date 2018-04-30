#include "esp_system.h"
#include <Fluotube.h>

const int button = 0;         //gpio to use to trigger delay
const int wdtTimeout = 3000;  //time in ms to trigger the watchdog
hw_timer_t *timer = NULL;

volatile int StateTOUCH;

void ISR_TOUCH()
{
  StateTOUCH = 1; 
}

void IRAM_ATTR resetModule() {
  ets_printf("reboot\n");
  esp_restart_noos();
}

void setup() {
  FluoTube.setup();
  FluoTube.debugln("running setup");

  touchAttachInterrupt(BUTTON_CHECK, ISR_TOUCH, 40);

  pinMode(button, INPUT_PULLUP);                    //init control pin
  timer = timerBegin(0, 80, true);                  //timer 0, div 80
  timerAttachInterrupt(timer, &resetModule, true);  //attach callback
  timerAlarmWrite(timer, wdtTimeout * 1000, false); //set time in us
  timerAlarmEnable(timer);                          //enable interrupt
}

void loop() {
  FluoTube.debugln("running main loop");

  timerWrite(timer, 0); //reset timer (feed watchdog)
  long loopTime = millis();
  
  //while button is pressed, delay up to 3 seconds to trigger the timer
  while (StateTOUCH) {
    FluoTube.debugln("button pressed");
    delay(500);

    StateTOUCH = 0;
  }

  delay(1000); //simulate work
  loopTime = millis() - loopTime;
  
  FluoTube.debug("loop time is = ");
  FluoTube.debugln(String(loopTime)); //should be under 3000
}