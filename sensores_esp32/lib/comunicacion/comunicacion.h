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

  String clientId = "";
  char cantFallos = 0;

  char mqtt_server[100] = {};//server
  char root_topic_subscribe[100] = {};//topic subscriber
  char root_topic_publish[100] = {};//topic publisher
  const int mqtt_port = 1883;//port
  const char *mqtt_user = "";//user
  const char *mqtt_pass = "";//pass

  public:

  	int cantDeFallasMQTT = 0;
	char flagConexionOK = 0;
	String ssid = "";
	String password = "";

    void setup_wifi(String, String);
    void setup_mqtt(String, String);
    void reconnect(void);
    void callback(char*, byte*, unsigned int);
    //void callback(char* topic, byte* payload, unsigned int length);
    bool publicarData(float);
    void loop(void);
	void setup_bluetooth(void);
	void serialBTprint(String);
	void serialBTprintln(String);
	void comprobarConexion(String,String,String,String);
	void setupModoRed(String, String, String, String);
	//int contadorDeFallasMQTT(bool);
	void switchCaseParametros(char, String);
	void cambioDeParametros(void);
	


};

class Parametros {
	public:
		String ssid = "";
		String password = "";

};