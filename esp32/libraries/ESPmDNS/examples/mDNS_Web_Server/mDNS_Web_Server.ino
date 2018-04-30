/*
  ESP32 mDNS responder sample

  This is an example of an HTTP server that is accessible
  via http://esp32.local URL thanks to mDNS responder.

  Instructions:
  - Update WiFi SSID and password as necessary.
  - Flash the sketch to the ESP32 board
  - Install host software:
    - For Linux, install Avahi (http://avahi.org/).
    - For Windows, install Bonjour (http://www.apple.com/support/bonjour/).
    - For Mac OSX and iOS support is built in through Bonjour already.
  - Point your browser to http://esp32.local, you should see a response.

 */


#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <Fluotube.h>

const char* ssid = "...";
const char* password = "...";

// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);

void setup(void)
{  
    FluoTube.setup();

    // Connect to WiFi network
    WiFi.begin(ssid, password);
    FluoTube.debugln("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        FluoTube.debug(".");
    }
    FluoTube.debugln("");
    FluoTube.debug("Connected to ");
    FluoTube.debugln( String(ssid) );
    FluoTube.debug("IP address: ");
    FluoTube.debugln( WiFi.localIP().toString() );

    // Set up mDNS responder:
    // - first argument is the domain name, in this example
    //   the fully-qualified domain name is "esp8266.local"
    // - second argument is the IP address to advertise
    //   we send our IP address on the WiFi network
    if (!MDNS.begin("esp32")) {
        FluoTube.debugln("Error setting up MDNS responder");
        while(1) {
            delay(1000);
        }
    }
    FluoTube.debugln("mDNS responder started");

    // Start TCP (HTTP) server
    server.begin();
    FluoTube.debugln("TCP server started");

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
}

void loop(void)
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    FluoTube.debugln("");
    FluoTube.debugln("New client");

    // Wait for data from client to become available
    while(client.connected() && !client.available()){
        delay(1);
    }

    // Read the first line of HTTP request
    String req = client.readStringUntil('\r');

    // First line of HTTP request looks like "GET /path HTTP/1.1"
    // Retrieve the "/path" part by finding the spaces
    int addr_start = req.indexOf(' ');
    int addr_end = req.indexOf(' ', addr_start + 1);
    if (addr_start == -1 || addr_end == -1) {
        FluoTube.debug("Invalid request: ");
        FluoTube.debugln( String(req) );
        return;
    }
    req = req.substring(addr_start + 1, addr_end);
    FluoTube.debug("Request: ");
    FluoTube.debugln( String(req) );
    client.flush();

    String s;
    if (req == "/")
    {
        IPAddress ip = WiFi.localIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP32 at ";
        s += ipStr;
        s += "</html>\r\n\r\n";
        FluoTube.debugln("Sending 200");
    }
    else
    {
        s = "HTTP/1.1 404 Not Found\r\n\r\n";
        FluoTube.debugln("Sending 404");
    }
    client.print(s);

    FluoTube.debugln("Done with client");
}

