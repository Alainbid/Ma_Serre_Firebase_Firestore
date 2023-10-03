#pragma once
#include <Time.h>
#include <clocale>
#include <NTPClient.h>
#include <WiFiUdp.h>

class ConfigurerDate
{
public:
   
   unsigned configurerDate(){

     //  décalage horaire
    const long utcOffset = 3600;
    // +1 h pour Paris
   
    // Define NTP Client pour obtenir la date du serveur web
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffset);
    timeClient.update();
   unsigned long maintenant = timeClient.getEpochTime();
    return maintenant;
 }
};


