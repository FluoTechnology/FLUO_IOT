/*
  FluoTube_WIFI.h - FluoTube library
  Copyright (c) 2017 Adriano Costanzo.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef FLUOTUBE_WIFI_H_
#define FLUOTUBE_WIFI_H_

#include <Arduino.h>

struct WiFi_t{
    String ssid;
    String passkey;
    String hostname;
    byte stored;

    String ipv4;
    String mac;
    byte connected;
};

struct Content_t{
    String data;
    int SeqNumber;
    byte ready;
};

struct WebSocket_t{
    String data;
    int sem;
};

extern struct WiFi_t WiFi_setting;
extern struct Content_t Content; 
extern struct WebSocket_t WebSocketSms; 

class FluoTubeClass_WIFI
{

	public:	

        String Setting(String, String, String);
        String ServerRest();
        String RestRead();
        int RestWrite(String data);

        int Available();

        // AUX
		String Info();
        String ScanWiFiNetwork();

        String ReadMacADDR();



	private:
	protected:
				
};

#endif /* FLUOTUBE_WIFI_H_ */
