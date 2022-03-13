#include "Arduino.h"
#include "WiFi.h"
#include "BluetoothSerial.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

//#ifndef SerialBT
  
//#endif
  

  

class Sensores {       // The class
  public:             // Access specifier
       
    String SSID;
    String Password;
    String url_broker;
    String topic;
    float value;
    String descriptor;
    String id;

};

class Network {


  char flagConexionOK = 0;

  String clientId = "";
  char cantFallos = 0;

  char mqtt_server[100] = {};//server
  char root_topic_subscribe[100] = {};//topic subscriber
  char root_topic_publish[100] = {};//topic publisher
  const int mqtt_port = 1883;//port
  const char *mqtt_user = "";//user
  const char *mqtt_pass = "";//pass

  public:
    void setup_wifi(String, String);
    void setup_mqtt(String, String);
    void reconnect(void);
    void callback(char*, byte*, unsigned int);
    //void callback(char* topic, byte* payload, unsigned int length);
    void publicarData(long);
    void loop(void);


};