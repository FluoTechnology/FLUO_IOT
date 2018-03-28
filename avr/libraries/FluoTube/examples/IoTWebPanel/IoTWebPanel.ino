#include <FluoTube.h>

void setup() {
  FluoTube.Begin();
}

void loop() {
  FluoTube.Run();

  delay(1000);
  Serial.println("ciao\r\n");
}
