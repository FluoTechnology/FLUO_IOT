#include "WiFi.h"
#include <Fluotube.h>

#define STA_SSID "**********"
#define STA_PASS "**********"
#define AP_SSID  "esp32"

static volatile bool wifi_connected = false;

WiFiUDP ntpClient;

void wifiOnConnect(){
    FluoTube.debugln("STA Connected");
    FluoTube.debug("STA IPv4: ");
    FluoTube.debugln( WiFi.localIP().toString() );
    
    ntpClient.begin(2390);
}

void wifiOnDisconnect(){
    FluoTube.debugln("STA Disconnected");
    delay(1000);
    WiFi.begin(STA_SSID, STA_PASS);
}

void wifiConnectedLoop(){
  //lets check the time
  const int NTP_PACKET_SIZE = 48;
  byte ntpPacketBuffer[NTP_PACKET_SIZE];

  IPAddress address;
  WiFi.hostByName("time.nist.gov", address);
  memset(ntpPacketBuffer, 0, NTP_PACKET_SIZE);
  ntpPacketBuffer[0] = 0b11100011;   // LI, Version, Mode
  ntpPacketBuffer[1] = 0;     // Stratum, or type of clock
  ntpPacketBuffer[2] = 6;     // Polling Interval
  ntpPacketBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  ntpPacketBuffer[12]  = 49;
  ntpPacketBuffer[13]  = 0x4E;
  ntpPacketBuffer[14]  = 49;
  ntpPacketBuffer[15]  = 52;
  ntpClient.beginPacket(address, 123); //NTP requests are to port 123
  ntpClient.write(ntpPacketBuffer, NTP_PACKET_SIZE);
  ntpClient.endPacket();

  delay(1000);
  
  int packetLength = ntpClient.parsePacket();
  if (packetLength){
    if(packetLength >= NTP_PACKET_SIZE){
      ntpClient.read(ntpPacketBuffer, NTP_PACKET_SIZE);
    }
    ntpClient.flush();
    uint32_t secsSince1900 = (uint32_t)ntpPacketBuffer[40] << 24 | (uint32_t)ntpPacketBuffer[41] << 16 | (uint32_t)ntpPacketBuffer[42] << 8 | ntpPacketBuffer[43];

    uint32_t epoch = secsSince1900 - 2208988800UL;

    uint8_t h = (epoch  % 86400L) / 3600;
    uint8_t m = (epoch  % 3600) / 60;
    uint8_t s = (epoch % 60);
    FluoTube.debugln("UTC: " + String(h) + ":" + String(m) + ":" + String(s) + "(GMT)");
  }

  delay(9000);
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {

        case SYSTEM_EVENT_AP_START:
            //can set ap hostname here
            WiFi.softAPsetHostname(AP_SSID);
            //enable ap ipv6 here
            WiFi.softAPenableIpV6();
            break;

        case SYSTEM_EVENT_STA_START:
            //set sta hostname here
            WiFi.setHostname(AP_SSID);
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            //enable sta ipv6 here
            WiFi.enableIpV6();
            break;
        case SYSTEM_EVENT_AP_STA_GOT_IP6:
            //both interfaces get the same event
            FluoTube.debug("STA IPv6: ");
            FluoTube.debugln( WiFi.localIPv6().toString() );
            FluoTube.debug("AP IPv6: ");
            FluoTube.debugln( WiFi.softAPIPv6().toString() );
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            wifiOnConnect();
            wifi_connected = true;
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            wifi_connected = false;
            wifiOnDisconnect();
            break;
        default:
            break;
    }
}

void setup(){

    FluoTube.setup();

    WiFi.disconnect(true);
    WiFi.onEvent(WiFiEvent);
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.softAP(AP_SSID);
    WiFi.begin(STA_SSID, STA_PASS);
}

void loop(){
    if(wifi_connected){
        wifiConnectedLoop();
    }
}
