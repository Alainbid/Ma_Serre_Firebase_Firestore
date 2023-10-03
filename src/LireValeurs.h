#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#endif
#include "DHTesp.h"
#define DHTTYPE DHT22 

class LireValeurs {
public :
// DHT 22  (AM2302)
DHTesp dht;


String untexte = "*";
double temperature = 0.0;
double humidity = 0.0;
bool debug = true;

 void  LireDht() {
    // IO14 = D5 détection température humidité
        dht.setup(14, DHTesp::DHT22);
    }
 //*******ACTUALISER
  // lecture  température et humidité du DHT22
    void actualiser()
    {
      char temp[5];
      temperature = dht.getTemperature();
      dtostrf(temperature, 0, 1, temp);
      delay(200);

      if (debug)
      {
        Serial.println(" ");
        Serial.print("capteur/température ");
        Serial.println(temp);
      }
      untexte.concat(" température = ");
      untexte.concat(temp);
      untexte.concat(" °C\n");

      humidity = dht.getHumidity();
      char humide[5];
      dtostrf(humidity, 0, 1, humide);
      delay(200);
      if (debug)
      {
        Serial.print("capteur/humidité ");
        Serial.println(humide);
        Serial.println();
      }
      untexte.concat("  humidité = ");
      untexte.concat(humide);
      untexte.concat("%");
    };//*********** fin de actualiser

};