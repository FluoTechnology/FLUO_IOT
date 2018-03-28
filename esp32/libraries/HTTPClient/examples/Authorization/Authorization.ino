/**
 * Authorization.ino
 *
 *  Created on: 09.12.2015
 *
 */

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <Fluotube.h>

#include <HTTPClient.h>

#define USE_SERIAL FluoTube

WiFiMulti wifiMulti;

void setup() {

    FluoTube.setup();

    delay(2000);

    wifiMulti.addAP("SSID", "PASSWORD");

}

void loop() {
    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.debugln("[HTTP] begin");
        // configure traged server and url

        http.begin("http://user:password@192.168.1.12/test.html");

        /*
          // or
          http.begin("http://192.168.1.12/test.html");
          http.setAuthorization("user", "password");

          // or
          http.begin("http://192.168.1.12/test.html");
          http.setAuthorization("dXNlcjpwYXN3b3Jk");
         */


        USE_SERIAL.debugln("[HTTP] GET");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.debugln("[HTTP] GET... code: " + String(httpCode) );

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.debugln( String(payload) );
            }
        } else {
            USE_SERIAL.debugln("[HTTP] GET... failed, error: " + String( http.errorToString(httpCode).c_str()) );
        }

        http.end();
    }

    delay(10000);
}

