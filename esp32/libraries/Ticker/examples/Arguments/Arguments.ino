#include <Arduino.h>
#include <Ticker.h>
#include <Fluotube.h>

// attach a LED to GPIO 13 (AVR side)
#define LED_PIN 13

Ticker tickerSetHigh;
Ticker tickerSetLow;

void setPin(int state) {
  FluoTube.digitalWrite(LED_PIN, state);
}

void setup() {

  FluoTube.setup();

  FluoTube.pinMode(LED_PIN, OUTPUT);
  
  // every 1000 ms, call setPin(0) 
  tickerSetLow.attach_ms(1000, setPin, 0);
  
  // every 1000 ms, call setPin(1)
  tickerSetHigh.attach_ms(1000, setPin, 1);
}

void loop() {

}
