#include <Arduino.h>
#include <Ticker.h>
#include <Fluotube.h>

// attach a LED to GPIO 13 (AVR side)
#define LED_PIN 13

Ticker blinker;
Ticker toggler;
Ticker changer;

float blinkerPace = 1;  //seconds
const float togglePeriod = 5; //seconds

void change() {
  blinkerPace = 0.5;
}

void blink() {
  FluoTube.digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void toggle() {
  static bool isBlinking = false;
  if (isBlinking) {
    blinker.detach();
    isBlinking = false;
  }
  else {
    blinker.attach(blinkerPace, blink);
    isBlinking = true;
  }
  FluoTube.digitalWrite(LED_PIN, LOW);  //make sure LED on on after toggling (pin LOW = led ON)
}

void setup() {

  FluoTube.setup();

  FluoTube.pinMode(LED_PIN, OUTPUT);

  toggler.attach(togglePeriod, toggle);
  changer.once(1000, change);
}

void loop() {
  
}
