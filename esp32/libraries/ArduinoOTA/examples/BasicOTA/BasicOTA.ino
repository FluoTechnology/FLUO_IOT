#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Fluotube.h>

const char* ssid = "..........";
const char* password = "..........";

void setup() {
  
  FluoTube.setup();
  
  FluoTube.debugln("Booting");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    FluoTube.debugln("Connection Failed");
    delay(5000);
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      FluoTube.debugln("Start updating: " + type);
    })
    .onEnd([]() {
      FluoTube.debugln("End");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      FluoTube.debug("Progress: ");
      int tmp = progress / (total / 100); 
      FluoTube.debugln( String(tmp) );
    })
    .onError([](ota_error_t error) {
      FluoTube.debug("Error: ");
      FluoTube.debugln( String(error) );
      if (error == OTA_AUTH_ERROR)          FluoTube.debugln("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)    FluoTube.debugln("Begin Failed");
      else if (error == OTA_CONNECT_ERROR)  FluoTube.debugln("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR)  FluoTube.debugln("Receive Failed");
      else if (error == OTA_END_ERROR)      FluoTube.debugln("End Failed");
    });

  ArduinoOTA.begin();

  FluoTube.debugln("Ready");
  
  FluoTube.debug("IP address: ");
  FluoTube.debugln( WiFi.localIP().toString() );
}

void loop() {
  ArduinoOTA.handle();
}
