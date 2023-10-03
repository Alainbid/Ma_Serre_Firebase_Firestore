//connexion WIFI



#define WIFI_SSID "michal"
#define WIFI_PASSWORD "bellecombe"
class ConWifi{

private:
 // friend void WiFiEventGotIp(WIFIEvent_t event, WIFIEventInfo_t info);

public:

ConWifi(){
  Serial.begin(115200);
 // WiFi.onEvent(WiFiEventStationModeConnected,WIFI_EVENT_STAMODE_CONNECTED);
//  WiFi.onEvent(WiFiEventStationModeDisconnected,WIFI_EVENT_STAMODE_DISCONNECTED);
 // WiFi.onEvent(WiFiEventGotIp,WIFI_EVENT_STAMODE_GOT_IP);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("\nConnecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(WiFi.status());
    delay(800);
  }
  Serial.print("\nConnecté  IP: \n");
  Serial.println( WiFi.localIP());

      }
};
  