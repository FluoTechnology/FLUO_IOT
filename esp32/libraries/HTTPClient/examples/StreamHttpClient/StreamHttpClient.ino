/**
 * StreamHTTPClient.ino
 *
 *  Created on: 24.05.2015
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

        // configure server and url
        http.begin("http://192.168.1.12/test.html");
        //http.begin("192.168.1.12", 80, "/test.html");

        USE_SERIAL.debugln("[HTTP] GET");
        // start connection and send HTTP header
        int httpCode = http.GET();
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.debugln("[HTTP] GET... code: " + String(httpCode) );

            // file found at server
            if(httpCode == HTTP_CODE_OK) {

                // get lenght of document (is -1 when Server sends no Content-Length header)
                int len = http.getSize();

                // create buffer for read
                uint8_t buff[128] = { 0 };

                // get tcp stream
                WiFiClient * stream = http.getStreamPtr();

                // read all data from server
                while(http.connected() && (len > 0 || len == -1)) {
                    // get available data size
                    size_t size = stream->available();

                    if(size) {
                        // read up to 128 byte
                        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                        // write it to Serial
                        USE_SERIAL.debug( String(buff) );

                        if(len > 0) {
                            len -= c;
                        }
                    }
                    delay(1);
                }

                USE_SERIAL.debugln();
                USE_SERIAL.debugln("[HTTP] connection closed or file end");

            }
        } else {
            USE_SERIAL.debugln("[HTTP] GET... failed, error: " + String( http.errorToString(httpCode).c_str()) );
        }

        http.end();
    }

    delay(10000);
}

