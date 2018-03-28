#include <SPI.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <Fluotube.h>

// CMD shell
// avrdude -v -c arduino -p atmega644P -P net:100.127.241.75:328 -U flash:w:Blink.ino.hex:i

#include <ESP32AVRISP.h>

const char* ssid     = "...";
const char* password = "...";
const char* host = "fluo";

const uint16_t port = 328;
const uint8_t reset_pin = RESET_644P;

ESP8266AVRISP avrprog(port, reset_pin, AVRISP_SPI_FREQ, false, true );
//uint16_t port, uint8_t reset_pin, uint32_t spi_freq, bool reset_state, bool reset_activehigh

void setup() {

    FluoTube.setup();
      
    digitalWrite(SPI_EN, 0); // 0 enable - 1 disable 

    SPI.begin(14,12,13);

    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        FluoTube.debug(".");
    }
    IPAddress local_ip = WiFi.localIP();

    FluoTube.debugln("");
    FluoTube.debug("Connected to ");
    FluoTube.debugln( String(ssid) );
    FluoTube.debug("IP address: ");
    FluoTube.debugln( String(local_ip) );

    if (!MDNS.begin(host)) {
        FluoTube.debugln("Error setting up MDNS responder");
        while(1) {
            delay(1000);
        }
    }
    FluoTube.debugln("mDNS responder started");

    MDNS.addService("http", "tcp", 80);
    MDNS.addService("avrisp", "tcp", port);

    FluoTube.debugln("Use your avrdude:");
    FluoTube.debug("avrdude -c arduino -p <device> -P net:");
    FluoTube.debug( String(local_ip) );
    FluoTube.debug(":");
    FluoTube.debug( String(port) );
    FluoTube.debugln(" -t # or -U ...");

    // listen for avrdudes
    avrprog.begin();
}

void loop() {

    static AVRISPState_t last_state = AVRISP_STATE_IDLE;
    AVRISPState_t new_state = avrprog.update();
    if (last_state != new_state) {
        switch (new_state) {
            case AVRISP_STATE_IDLE: {
                FluoTube.debugln("[AVRISP] now idle");
                // Use the SPI bus for other purposes
                break;
            }
            case AVRISP_STATE_PENDING: {
                FluoTube.debugln("[AVRISP] connection pending");
                // Clean up your other purposes and prepare for programming mode
                break;
            }
            case AVRISP_STATE_ACTIVE: {
                FluoTube.debugln("[AVRISP] programming mode");
                // Stand by for completion
                break;
            }
        }
        last_state = new_state;
    }
    // Serve the client
    if (last_state != AVRISP_STATE_IDLE) {
        avrprog.serve();
    }
 
}
