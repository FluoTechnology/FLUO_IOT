/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Fluotube.h>

int scanTime = 30; //In seconds

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      FluoTube.debug("Advertised Device: ");
      FluoTube.debug( String(advertisedDevice.toString().c_str()) );
    }
};

void setup() {

  FluoTube.setup();
  FluoTube.debugln("Scanning...");

  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  BLEScanResults foundDevices = pBLEScan->start(scanTime);
  FluoTube.debug("Devices found: ");
  FluoTube.debugln( String(foundDevices.getCount()) );

  FluoTube.debugln("Scan done! ");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}