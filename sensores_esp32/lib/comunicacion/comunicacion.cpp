#include "comunicacion.h"

#define LED_ONBOARD 2

//Network call;
BluetoothSerial SerialBT;
WiFiClient espClient1;
PubSubClient client1(espClient1);


void Network::setup_wifi(String ssid, String password){

      int cuenta = 0;

      // Nos conectamos a nuestra red Wifi
      Serial.println();
      Serial.print("Conectando a ssid: ");
      Serial.println(ssid);


      const char* ssidConverted = ssid.c_str();
      const char* passwordConverted = password.c_str();
        
      WiFi.begin(ssidConverted, passwordConverted);

      while ((WiFi.status() != WL_CONNECTED) && cuenta < 20) {//límite de 20 intentos de 500 ms
        delay(500);
        Serial.print(".");
        cuenta++;
      
      }
      if(WiFi.status() != WL_CONNECTED){//si no logró conectarse
      
        Serial.println("No es posible conectar a WiFi");
        Serial.println("Se cambia a MODO LOCAL");
        SerialBT.println("No es posible conectar a WiFi");
        SerialBT.println("Se cambia a MODO LOCAL");

      }else{//si logró conectarse

        Serial.println("");
        Serial.println("Conectado a red WiFi!");
        Serial.println("Dirección IP: ");
        SerialBT.println("Conectado a red WiFi!");
        SerialBT.println("Dirección IP: ");
        Serial.println(WiFi.localIP());
        SerialBT.println(WiFi.localIP());
        delay(5000);
      }
}

void Network::callback(char* topic, byte* payload, unsigned int length){
  
  String incoming = "";
  char charParamID = ' ';
  String valorParam = "";
  //int inChar = 0;
  
  

  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");

  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }

  incoming.trim();
  Serial.println("Mensaje -> " + incoming);

  //obtiene el identificador
  charParamID = incoming.charAt(0);

  if(charParamID == 'P'){
	  Serial.println("Llegó una P");
	  digitalWrite(LED_ONBOARD, HIGH);
  }else if(charParamID == 'A'){
	  Serial.println("Llegó una A");
	  digitalWrite(LED_ONBOARD, LOW);
  }
  
  Serial.println(charParamID);
  
  //obtiene el valor
  for(int i = 1; i < incoming.length(); i++){
    valorParam += incoming.charAt(i);
  }

  Serial.println(valorParam);



  //evalua el identificador y los parámetros enviados
//  switchCaseParametros(charParamID, valorParam);

  //borra el contenido y lo prepara para recibir uno nuevo
  incoming = "";

}

void Network::setup_mqtt(String broker, String topic){

    //callback call = callback();

    
    strcpy(mqtt_server, broker.c_str());

    //mqtt_port = 1883;//port
    mqtt_user = "pentium";//user
    mqtt_pass = "7364";//pass

    
    strcpy(root_topic_subscribe, topic.c_str());

    
    strcpy(root_topic_publish, topic.c_str());

    /*
    //char root_topic_subscribe[100] =     topic.c_str();//"undefi";
    char root_topic_publish[100] = "INTI/Electronica/esp32/sensor_1";//"undefi";
    char tempAmbiente_topic_subscribe[100] = "INTI/Electronica/esp32/sensor_1";//"undefi/handtemp/1/tempAmbiente";
    char tempAmbiente_topic_publish[100] = "INTI/Electronica/esp32/sensor_1";//"undefi/handtemp/1/tempAmbiente";
    char tempObjeto_topic_subscribe[100] = "INTI/Electronica/esp32/sensor_1";//"undefi/handtemp/1/tempObjeto";
    char tempObjeto_topic_publish[100] = "INTI/Electronica/esp32/sensor_1";//"undefi/handtemp/1/tempObjeto";
    char data_topic_publish[100] = "INTI/Electronica/esp32/data/sensor_1";//"undefi/handtemp/1/tempObjeto";
    char handtempKeepAlive_topic_publish[100] = "INTI/Electronica/esp32/keepAlive/sensor_1";
    */




    client1.setServer(mqtt_server, mqtt_port);//inicializa server en broker local
	//client1.setServer("broker.hivemq.com", mqtt_port);//inicializa server en broker local
    //client1.setCallback(callback);

    //intenta conectar con MQTT
    if(!client1.connected()){
      reconnect();//si no está conectado, lo reconecta
    }
    if(client1.connected()){//si logró reconectarse o ya estaba conectado
      Serial.println("Conexión OK: Wifi y MQTT conectados");
      Serial.println("MODO RED");
          SerialBT.println("Conexión OK: Wifi y MQTT conectados");
      SerialBT.println("MODO RED");
      //flagCambioModoLocal = 0;//no hace falta cambiar a modo local
      flagConexionOK = 1;
      
    }else{
      Serial.println("Conexión Errónea: Wifi y MQTT no conectados");
      Serial.println("MODO LOCAL...(temporal)");
          SerialBT.println("Conexión Errónea: Wifi y MQTT no conectados");
      SerialBT.println("MODO LOCAL...(temporal)");
      //flagCambioModoLocal = 1;
      //flagModoRed = 0;
      flagConexionOK = 0;
    }


}


//*****************************
//***    CONEXION MQTT      ***
//*****************************



void Network::reconnect(void) {

  //alimentar watchDog
  //esp_task_wdt_reset();
  //flagCambioModoLocal = 0;
  int resultado = 0;

  while (!client1.connected() == 1 && flagConexionOK == 0) {
    Serial.println("Intentando conexión Mqtt1...");
    SerialBT.println("Intentando conexión Mqtt1...");
    /*
    Serial.print("mqtt_server = ");
    Serial.println(mqtt_user);
    Serial.print("mqtt_pass = ");
    Serial.println(mqtt_pass);
    */
    // Creamos un cliente ID
    clientId = "IOTICOS_H_W_";
    clientId += String(random(0xffff), HEX);
    //String clientId = "UNDEFI_handtemp";
    // Intentamos conectar
    resultado = client1.connect(clientId.c_str(),mqtt_user,mqtt_pass);
    Serial.print("resultado: ");
    Serial.print(resultado);
    if(resultado == 1){
      Serial.println("Conectado!");
      SerialBT.println("Conectado!");
      flagConexionOK = 1;
	  if(client1.subscribe(root_topic_subscribe)){
        Serial.println("Suscripcion ok");
        SerialBT.println("Suscripcion ok");
	  }else{
        Serial.println("fallo Suscripción");
        SerialBT.println("fallo Suscripción");
      }
    
    }else{
      
      cantFallos++;
      Serial.print(" cantFallos: ");
      Serial.print(cantFallos);
      Serial.print(" falló :( con error -> ");
      Serial.print(client1.state());
      Serial.println(" Intentamos de nuevo en 1 segundo");
      delay(1000);

      if(cantFallos == 2){
        cantFallos = 0;
        flagConexionOK = 0;
        break;
        
      }
    }
  }
}


bool Network::publicarData(float dato){



//***************VER de convertir a JSON 6 ******************

	// ArduinoJson 6
	// DynamicJsonDocument doc(1024);
	// doc["key"] = "value";
	// doc["raw"] = serialized("[1,2,3]");
	// serializeJson(doc, Serial);


        
  //prepara el objeto JSON para publicar por MQTT
  //StaticJsonBuffer<300> JSONbuffer;
  //StaticJsonDocument<300> JSONbuffer;
  //JsonObject JSONencoder = JSONbuffer.createObject();
 
  //JSONencoder["dato"] = round(dato * 100) / 100;
  /*
  char JSONmessageBuffer[300];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("Enviando data por MQTT1...");
  Serial.println(JSONmessageBuffer);
  //WebSerial.println(JSONmessageBuffer);
*/
  //client1.publish(root_topic_publish, JSONmessageBuffer);
  //client1.publish(root_topic_publish, "25.00");
	bool pubOK = 0;

	// ArduinoJson 6
	DynamicJsonDocument doc(1024);
	String docSerializado = {};
	      
	//prepara el objeto JSON para publicar por MQTT
	doc["dato"] = round(dato * 100) / 100;
	serializeJson(doc, docSerializado);//le da formato JSON  
	Serial.println("Enviando data por MQTT1...");

	Serial.println(docSerializado);
	 //WebSerial.println(docSerializado);

	pubOK = client1.publish(root_topic_publish, docSerializado.c_str());
	return pubOK;



}

void Network::loop(void){

  client1.loop();

}

void Network::setup_bluetooth(void){

	SerialBT.begin();
	SerialBT.println("Bluetooth service has started");


}

void Network::serialBTprint(String str){

	SerialBT.print(str);

}

void Network::serialBTprintln(String str){

	SerialBT.println(str);
	
}