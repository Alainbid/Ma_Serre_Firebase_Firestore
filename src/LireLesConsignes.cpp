#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#include "LireLesConsignes.h"
#define FIREBASE_PROJECT_ID "cfbconta"


LireLesConsignes::Consignes LireLesConsignes::lirelesconsignes() {
    StaticJsonDocument<800> doc;
    FirebaseJson content;
    FirebaseData fbdo;
    // unsigned int duree_Arrosage = 10 * 1000; // par défaut 10s d'arrosage
    // unsigned int frequence_Arrosage = 4*60*60*1000;    //par défaut  toutes les 4 heures
    //initialization à 0,0
    Consignes consignes = {0, 0};

    if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", "consigner/consigne", content.raw()))
    {
      Serial.printf("Lecture === OK\n%s\n\n", fbdo.payload().c_str());
      String l = fbdo.payload().c_str();
      DeserializationError error = deserializeJson(doc, fbdo.payload().c_str());
      Serial.printf("\n length = %d\n", sizeof(l));

      // Test if parsing succeeds.
      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      }
      else
      {
        String nom = doc["name"];
        Serial.println(nom);
        String no = doc["fields"];
        Serial.println(no);
        // durée est stockée en mn doit etre convertie en millis
        double dur = doc["fields"]["duree"]["doubleValue"];
        // duree_Arrosage = (dur * 60 *1000);
        consignes.duree = (dur * 60 * 1000);

        // la frequence stockée en heure convertie en millis
        double fre = doc["fields"]["frequence"]["doubleValue"];
        consignes.frequence = fre * (60 * 60 * 1000); // en millis
        // frequence_Arrosage = fre *(60*60*1000);// en millis

        Serial.printf("---- duree = %.1d millis\n", consignes.duree);
        Serial.printf("---- freq = %.1d millis\n", consignes.frequence);
        // date de la dernière mise à jour
        String d = doc["createTime"];
        Serial.println(d);
      }
    }
    else
    {
      Serial.println("*****Erreur consigne*********" + fbdo.errorReason());
    }
    
    return consignes;
  }
