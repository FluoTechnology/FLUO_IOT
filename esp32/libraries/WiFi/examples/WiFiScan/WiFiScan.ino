/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"
#include <Fluotube.h>

void setup()
{
    FluoTube.setup();

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    FluoTube.debugln("Setup done");
}

void loop()
{
    FluoTube.debugln("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    FluoTube.debugln("scan done");
    if (n == 0) {
        FluoTube.debugln("no networks found");
    } else {
        FluoTube.debug( String(n) );
        FluoTube.debugln(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found

            FluoTube.debug( String(i + 1) );
            Serial.print(": ");
            Serial.print( String(WiFi.SSID(i)) );
            Serial.print(" (");
            Serial.print( String(WiFi.RSSI(i)) );
            Serial.print(")");
            Serial.println(  String( (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*") );

            delay(10);
        }
    }
    FluoTube.debugln("");

    // Wait a bit before scanning again
    delay(5000);
}
