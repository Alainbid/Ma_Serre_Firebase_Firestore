#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#define FIREBASE_PROJECT_ID "cfbconta"

struct Consignes {
  int frequence = 0;
  int duree = 0;
};
 
 class LireLesConsignes {

public:
    bool LectureTerminee = false;

   void  lirelesconsignes() {
    StaticJsonDocument<800> doc;
    FirebaseJson content;
    FirebaseData fbdo;

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
       consignes.duree = dur *( 60 * 1000);

        // la frequence stockée en heure convertie en millis
        double fre = doc["fields"]["frequence"]["doubleValue"];
      consignes.frequence = fre* (60 * 60 * 1000); // en millis

         Serial.printf("---------------- durée = %.1d millis\n", consignes.duree);
         Serial.printf("---------------- fréquence = %.1d millis\n", consignes.frequence);
         // date de la dernière mise à jour
         String d = doc["createTime"];
         Serial.println(d);
         LectureTerminee = true;
      }
    }
    else
    {
      Serial.println("*****Erreur consigne*********" + fbdo.errorReason());
    }

    
   }

  public:

  static Consignes consignes;
  
 };

 Consignes LireLesConsignes::consignes;
 