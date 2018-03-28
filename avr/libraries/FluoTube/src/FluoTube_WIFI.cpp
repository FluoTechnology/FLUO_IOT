/*
  FluoTube_WIFI.cpp - FluoTube library
  Copyright (c) 2017 Adriano Costanzo.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "FluoTube_WIFI.h"

//#define DEBUG

struct WiFi_t WiFi_setting = {"ssid", "pass", "hostname", 0, "null", "null", 0};
struct Content_t Content = {"data", 0, 0};


String FluoTubeClass_WIFI::Setting(String ssid, String key, String host)
{
    WiFi_setting.ssid = ssid;
    WiFi_setting.passkey = key;
    WiFi_setting.hostname = host;

    WiFi_setting.stored = 2; 
    
    // lo metto a 2 cosi FluoTube.c sa che e' arrivata una nuova conf.

    return "Stored!"; 	
}



String FluoTubeClass_WIFI::ServerRest()
{
    return "Attiva il Server REST sulla porta 8080";    
}

String FluoTubeClass_WIFI::RestRead()
{
    static byte oneshot = 0;
    
    if ( Content.ready == 0 && oneshot == 0)
    {   
        oneshot = 1;
        return "ADDR";
    }

    if ( Content.ready == 0 )
        return "";

    if ( Content.ready == 1 )
    {
        Content.ready = 0;
        return Content.data;
    }   
}


int FluoTubeClass_WIFI::Available()
{
    #ifdef DEBUG
    if (WiFi_setting.stored == 0)
        Serial.println("Error WIFI Credentials");
    #endif

    if (WiFi_setting.connected == 1)
        return 1; // if connection is established 

    return 0; //  no connection! 
}

String FluoTubeClass_WIFI::ScanWiFiNetwork()
{
    return "";  
}

String FluoTubeClass_WIFI::Info()
{
    String url;
    
    url += "http://";
    url += WiFi_setting.ipv4;
    url += ":8080/";

    return url;  
}

String FluoTubeClass_WIFI::ReadMacADDR()
{
    return WiFi_setting.mac;  
}
