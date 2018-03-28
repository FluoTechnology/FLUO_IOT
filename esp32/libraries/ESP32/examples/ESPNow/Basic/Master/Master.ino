/**
   ESPNOW - Basic communication - Master
   Date: 26th September 2017
   Author: Arvind Ravulavaru <https://github.com/arvindr21>
   Purpose: ESPNow Communication between a Master ESP32 and a Slave ESP32
   Description: This sketch consists of the code for the Master module.
   Resources: (A bit outdated)
   a. https://espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
   b. http://www.esploradores.com/practica-6-conexion-esp-now/

   << This Device Master >>

   Flow: Master
   Step 1 : ESPNow Init on Master and set it in STA mode
   Step 2 : Start scanning for Slave ESP32 (we have added a prefix of `slave` to the SSID of slave for an easy setup)
   Step 3 : Once found, add Slave as peer
   Step 4 : Register for send callback
   Step 5 : Start Transmitting data from Master to Slave

   Flow: Slave
   Step 1 : ESPNow Init on Slave
   Step 2 : Update the SSID of Slave with a prefix of `slave`
   Step 3 : Set Slave in AP mode
   Step 4 : Register for receive callback and wait for data
   Step 5 : Once data arrives, print it in the serial monitor

   Note: Master and Slave have been defined to easily understand the setup.
         Based on the ESPNOW API, there is no concept of Master and Slave.
         Any devices can act as master or salve.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <Fluotube.h>

// Global copy of slave
esp_now_peer_info_t slave;
#define CHANNEL 3
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0

// Init ESP Now with fallback
void InitESPNow() {
  if (esp_now_init() == ESP_OK) {
    FluoTube.debugln("ESPNow Init Success");
  }
  else {
    FluoTube.debugln("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    //ESP.restart(); danger !
  }
}

// Scan for slaves in AP mode
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();
  // reset on each scan
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  FluoTube.debugln("");
  if (scanResults == 0) {
    FluoTube.debugln("No WiFi devices in AP Mode found");
  } else {
    FluoTube.debug("Found "); FluoTube.debug( String(scanResults) ); FluoTube.debugln(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (PRINTSCANRESULTS) {
        FluoTube.debug( String(i + 1) );
        FluoTube.debug(": ");
        FluoTube.debug( String(SSID) );
        FluoTube.debug(" (");
        FluoTube.debug( String(RSSI) );
        FluoTube.debug(")");
        FluoTube.debugln("");
      }
      delay(10);
      // Check if the current device starts with `Slave`
      if (SSID.indexOf("Slave") == 0) {
        // SSID of interest
        FluoTube.debugln("Found a Slave.");
        FluoTube.debug( String(i + 1) ); 
        FluoTube.debug(": "); 
        FluoTube.debug( String(SSID) ); 
        FluoTube.debug(" ["); 
        FluoTube.debug( String(BSSIDstr) ); 
        FluoTube.debug("]"); 
        FluoTube.debug(" ("); 
        FluoTube.debug( String(RSSI) ); 
        FluoTube.debug(")"); 
        FluoTube.debugln("");

        // Get BSSID => Mac Address of the Slave
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x%c",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = CHANNEL; // pick a channel
        slave.encrypt = 0; // no encryption

        slaveFound = 1;
        // we are planning to have only one slave in this example;
        // Hence, break after we find one, to be a bit efficient
        break;
      }
    }
  }

  if (slaveFound) {
    FluoTube.debugln("Slave Found, processing.. ");
  } else {
    FluoTube.debugln("Slave Not Found, trying again. ");
  }

  // clean up ram
  WiFi.scanDelete();
}

// Check if the slave is already paired with the master.
// If not, pair the slave with master
bool manageSlave() {
  if (slave.channel == CHANNEL) {
    if (DELETEBEFOREPAIR) {
      deletePeer();
    }

    FluoTube.debug("Slave Status: ");
    const esp_now_peer_info_t *peer = &slave;
    const uint8_t *peer_addr = slave.peer_addr;
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(peer_addr);
    if ( exists) {
      // Slave already paired.
      FluoTube.debugln("Already Paired");
      return true;
    } else {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(peer);
      if (addStatus == ESP_OK) {
        // Pair success
        FluoTube.debugln("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        FluoTube.debugln("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        FluoTube.debugln("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        FluoTube.debugln("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        FluoTube.debugln("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        FluoTube.debugln("Peer Exists");
        return true;
      } else {
        FluoTube.debugln("Not sure what happened");
        return false;
      }
    }
  } else {
    // No slave found to process
    FluoTube.debugln("No Slave found to process");
    return false;
  }
}

void deletePeer() {
  const esp_now_peer_info_t *peer = &slave;
  const uint8_t *peer_addr = slave.peer_addr;
  esp_err_t delStatus = esp_now_del_peer(peer_addr);

  FluoTube.debug("Slave Delete Status: ");
  if (delStatus == ESP_OK) {
    // Delete success
    FluoTube.debugln("Success");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    FluoTube.debugln("ESPNOW Not Init");
  } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
    FluoTube.debugln("Invalid Argument");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
    FluoTube.debugln("Peer not found.");
  } else {
    FluoTube.debugln("Not sure what happened");
  }
}

uint8_t data = 0;
// send data
void sendData() {
  data++;
  const uint8_t *peer_addr = slave.peer_addr;
  FluoTube.debug("Sending: "); 
  FluoTube.debugln( String(data) );

  esp_err_t result = esp_now_send(peer_addr, &data, sizeof(data));
  FluoTube.debug("Send Status: ");
  if (result == ESP_OK) {
    FluoTube.debugln("Success");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    FluoTube.debugln("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    FluoTube.debugln("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    FluoTube.debugln("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    FluoTube.debugln("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    FluoTube.debugln("Peer not found.");
  } else {
    FluoTube.debugln("Not sure what happened");
  }
}

// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

  FluoTube.debug("Last Packet Sent to: "); 
  FluoTube.debugln( String(macStr) );
  FluoTube.debug("Last Packet Send Status: "); 
  FluoTube.debugln( String( status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail") );
}

void setup() {

  FluoTube.setup();

  //Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  FluoTube.debugln("ESPNow/Basic/Master Example");
  // This is the mac address of the Master in Station Mode
  FluoTube.debug("STA MAC: "); 
  FluoTube.debugln( String(WiFi.macAddress()) );

  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  // In the loop we scan for slave
  ScanForSlave();
  // If Slave is found, it would be populate in `slave` variable
  // We will check if `slave` is defined and then we proceed further
  if (slave.channel == CHANNEL) { // check if slave channel is defined
    // `slave` is defined
    // Add slave as peer if it has not been added already
    bool isPaired = manageSlave();
    if (isPaired) {
      // pair success or already paired
      // Send data to device
      sendData();
    } else {
      // slave pair failed
      FluoTube.debugln("Slave pair failed!");
    }
  }
  else {
    // No slave found to process
  }

  // wait for 3seconds to run the logic again
  delay(3000);
}