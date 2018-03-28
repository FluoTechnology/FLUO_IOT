// ESP32 Touch Test
// Just test touch pin - Touch is BUTTON_CHECK (GPIO27).

#include <Fluotube.h>

void setup()
{
  FluoTube.setup();

  delay(1000); // give me time to bring up serial monitor
  FluoTube.debugln("ESP32 Touch Test");
}

void loop()
{
  FluoTube.debugln( String(touchRead(BUTTON_CHECK)) );  // get value using T0
  delay(1000);
}
