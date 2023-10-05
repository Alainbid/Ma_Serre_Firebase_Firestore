#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#endif

#include <Firebase_ESP_Client.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include "DHTesp.h"
#include <Time.h>
#include <clocale>
#include <string.h>
#include <ArduinoJson.h>
#include "LireLesConsignes.h"
#include "connexion_WIFI.h"
#include "Configurer_Date.h"
#include "LireValeurs.h"
//******* PROJET DE CONTROLE ESP8266 PAR WEB ET FIREBASE
//******* Lecture des valeurs de frequence et duree dans FIREBASE
//******* A chaque arrosage enregistrement date et freq dure temp hum
//******* Interagit avec MASERREDERNIEREVERSION pour majour freq duree
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define USER_EMAIL "alainbidault56@gmail.com"
#define USER_PASSWORD "Xccobb11mai68"

// Projets CFBconta
// Insert Firebase project API Key
#define API_KEY "AIzaSyA2duUPsmzfETLcHBuRTl_NtF8FkBp2VE4"
#define DATABASE_URL "https://cfbconta.firebaseio.com"
#define FIREBASE_PROJECT_ID "cfbconta"

bool debug = true;

// Create an instance of the LireLesConsignes class

 LireLesConsignes lireconsignes;

LireValeurs actualise;


const char *PARAM_INPUT_1 = "frequence";
const char *PARAM_INPUT_2 = "duree";
// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson content;
FirebaseJson nouveau;

// declarations des paramètres
unsigned long sendDataPrevMillis = 0L;
unsigned long maintenant = 0L;
bool signupOK = false;
bool debut = true; // si debut on fait un premier cycle d arrosage
int relai = 5;     // relai d'arrosage
int led = 2;       // led  ON si  relai OFF
unsigned long depart = millis();
unsigned long debut_Arrosage = 0;
String documentPath = "serre";
char quand[29];

Consignes valeurs;
LireLesConsignes  termine;

//****************** ECRIRE  dans FIREBASE
void ecriredb()
{
  //***********stockage provisoire  si debug = true
  if (debug)
  {
    documentPath = "adebug";
  }

  // lecturetempérature humidité dans ESP8266
  actualise.LireDht();
  actualise.actualiser();
  double temperature = actualise.temperature;
  double humidity = actualise.humidity;

  // actualisation de la date en timestamp
  ConfigurerDate cfgdt;
  maintenant = cfgdt.configurerDate();
  // on se limite à des minutes soit 8 digits
  snprintf(quand, 9, "%ld", maintenant / (60 * 1000));
  if (debug)
  {
    printf("contenu de quand %s FB date en mn\n\n", quand);
  }

  // création du fichier JSON contenant les données
  nouveau.set("fields/temperature/doubleValue", String(temperature).c_str());
  nouveau.set("fields/humidite/doubleValue", String(humidity).c_str());
  double freq = valeurs.frequence / 3600000.0;
  // double freq = 1.1;
  // les frequences sont en millis conversion en h.
  nouveau.set("fields/frequence/doubleValue", freq);
  // les durée arrosage sont en millis conversion en mn
  double dur = valeurs.duree / 60000.0;
  // double dur = 1.11 ;
  nouveau.set("fields/duree/doubleValue", dur);
  nouveau.set("fields/date/doubleValue", String(maintenant).c_str()); // date en millis
  if (debug)
  {
    Serial.printf("\nduree =%e \n", dur);
    Serial.printf("\nfrequence =%e \n", freq);
  }

  //*********création d'un nouveau document dans Firebase
  if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath, nouveau.raw()))
  {
    Serial.printf("OK creation \n");
    Serial.printf("Document créé : %s\n\n", fbdo.payload().c_str());
    return;
  }
  else
  {
    Serial.println("**create ERREUR *******" + fbdo.errorReason());
  }
} //****** fin ecriredb  **********************************************

//********  SETUP  ******************************************************************
void setup()
{

  pinMode(led, OUTPUT);    // set the LED pin mode
  digitalWrite(led, HIGH); // la led du ESP8266 doit être à 0 pour allumée
  pinMode(relai, OUTPUT);
  // dht.setup(14, DHTesp::DHT22); // IO14 = D5 détection température humidité

  // connexion WIFI  dans connexion_WIFI.h
  ConWifi();

  //*********  connection à FIREBASE ***********************************
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  // autres composants de configuration

  fbdo.setResponseSize(4096);
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;
  // connexion à FIREBASE
  Firebase.begin(&config, &auth);

  Serial.println("Obtention du User UID");
  while ((auth.token.uid) == "")
  {
    Serial.print('.');
    delay(500);
  }
  // on est connecté à FIREBASE fbconta realtime database
  //  "Ma-Serre-Firebase-Firestore"
  String uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.print(uid);
  signupOK = true;
  Serial.printf("\nFirebase authenticated v%d\n\n", Firebase.authenticated());

  // Lire  Les Consignes dans firebase
  lireconsignes.lirelesconsignes();
  if (termine.LectureTerminee)
  {
    Serial.printf("---- durée = %.1d millis\n", valeurs.duree);
    Serial.printf("---- fréquence = %.1d millis\n", valeurs.frequence);

    if (debug)
    {
      valeurs.duree = 10000;     // 10 seconde = 10000 millis
      valeurs.frequence = 60000; // 1 mn = 60000 mills
    }
  }
  else
  {
    Serial.println("=========================Pas de lecture des consignes");
  }

} // fin du SETUP ********************************************************

//*********************************************************************
void loop()
{
  unsigned long deb = valeurs.frequence; // conversion int --> unsigned long
  if (Firebase.ready() && signupOK &&
      ((debut) || (millis() - depart > deb)))
  // fin de la fréquence arrosage ... on arrose
  {
    debut = false;
    // début arrosage pour une durée de duree
    if (debug)
    {
      snprintf(quand, 9, "%d", valeurs.duree / 1000); // duree arrosage en millisecondes ramenées en s
      Serial.printf("\ndans la boucle arrosage pour %s \n", quand);
    }
    digitalWrite(relai, HIGH); // arrosage
    digitalWrite(led, LOW);
    debut_Arrosage = millis(); // date du debut arosage

    // dans la boucle d'arrosage pendant durée arrosage
    while (millis() < (debut_Arrosage + valeurs.duree))
    {
      Serial.print(".");
      delay(200);
    }
    digitalWrite(relai, LOW);
    digitalWrite(led, HIGH);
    if (debug)
    {
      Serial.printf("\n fin arrosage\n");
    }
    // verification de la  connexion wifi
    // reconnecter si coupure
    if (WiFi.status() != WL_CONNECTED)
    {
      ConWifi();
    }

    // relire les valeurs de consignes
    //  au cas ou elles auraient été modifiées entre temps
    if (!debug)
      LireLesConsignes lirelesconsignes();

    // écriture des valeurs dans FIREBASE
    ecriredb();

    // raz du compteur de fréquence arrosage
    depart = millis();
  }

} // fin de LOOP *****************************************************************
