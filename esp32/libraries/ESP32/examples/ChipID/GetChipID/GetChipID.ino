#include <Fluotube.h>

uint64_t chipid;  

void setup() {
	FluoTube.setup();
}

void loop() {

	chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).

	FluoTube.debug("ESP32 Chip ID = ");
    FluoTube.debugln( String ( (uint16_t)(chipid>>32) ) );//print High 2 bytes

    FluoTube.debugln( String ( (uint32_t)chipid) );//print Low 4bytes.

	delay(3000);
}
