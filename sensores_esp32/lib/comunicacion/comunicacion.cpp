#include "comunicacion.h"

#define LED_ONBOARD 2

//Network call;
BluetoothSerial SerialBT;
WiFiClient espClient1;
PubSubClient client1(espClient1);
//Sensores sensor1;


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

	bool pubOK = 0;

	// ArduinoJson 6
	DynamicJsonDocument doc(1024);
	String docSerializado = {};
	      
	//prepara el objeto JSON para publicar por MQTT
	doc["dato"] = round(dato * 100) / 100;
	serializeJson(doc, docSerializado);//le da formato JSON  
	Serial.println("Enviando data por MQTT1...");

	Serial.println(docSerializado);

	pubOK = client1.publish(root_topic_publish, docSerializado.c_str());

	if(pubOK == 0){//si falló la publicación lo contabiliza
		cantDeFallasMQTT++;
	}


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
/*
int Network::contadorDeFallasMQTT(bool pubOK){
	if(pubOK == 0){
		cantDeFallasMQTT++;
	}
	return cantDeFallasMQTT;
}
*/

void Network::comprobarConexion(String ssid,String pass,String broker,String topic){

  setupModoRed(ssid,pass,broker,topic);//configura MQTT, revisa conectividad
 
}

void Network::setupModoRed(String ssid,String pass,String broker,String topic){

	Network::setup_wifi(ssid,pass);
	Network::setup_mqtt(broker,topic);

}

void Network::switchCaseParametros(char charParamID, String valorParam){

  int inChar = 0;
  int index = 0;
  int valorParamLength = 0;
  int endIndex = 0;
  int modoDebug = 0;
  int consultarLecturas = 0;
  int correccionActivada = 0;
  uint8_t numSensor = 0;
  uint16_t direccion = 0;
  int scanActivado = 0;
  byte oldAddress = 0;
  byte newAddress = 0;
  int analizarLecturasCantidad = 0;
  int intercambioSensores = 0;
  int color = 0;
  String nombreSensor = "";
  String ssid = "";
  String password = "";
  
  //valorParam = 
  valorParam.replace(0x0A,'\0');//Se filtra el caracter LF
  valorParam.replace(0x0D,'\0');//Se filtra el caracter CR

  switch(charParamID){
   
    case 'W':

      Serial.println("Wifi: ");
      valorParamLength = strlen(valorParam.c_str());
      endIndex = valorParamLength;
      index = valorParam.indexOf(' ');
      ssid = valorParam.substring(0, index);
      Serial.println(ssid);
      //password = valorParam.substring(index + 1, endIndex - 1);
      password = valorParam.substring(index + 1, endIndex);
      Serial.println(password);
      /*
	  //guarda config wifi en EEPROM
      EEPROM.writeString(EEPROM_ADDRESS_WIFI_SSID, ssid);
      EEPROM.commit();
      EEPROM.writeString(EEPROM_ADDRESS_WIFI_PASS, password);
      EEPROM.commit();
	  */
      setup_wifi(ssid, password);

    break;
   
    default:
      Serial.println("Parámetro incorrecto");
    break;

  }  
}

//puede cambiar parámetros a través del puerto serie o por bluetooth
//Se debe enviar un caracter de identificación del parámetro a cambiar y
//luego el valor.
//Por ejemplo: cambiar el tiempo entre lecturas de temperatura
//enviar T100  siendo T: tiempoEntreLecturas; 100: 100 ms
//los parámetros que se pueden modificar son:
//  Wifi--> W;  [Ejemplo: Wmyssid mypassword](El espacio se usa como delimitador)
void Network::cambioDeParametros(void){

  char charParamID = ' ';
  String valorParam = "";
  int inChar = 0;
  String inString = "";
    
  
  //**** Chequeo por Serie o Bluetooth ***************
  while (Serial.available() > 0 || SerialBT.available() > 0) {

    if(Serial.available() > 0){
      inChar = Serial.read();
    }else if(SerialBT.available() > 0){
      inChar = SerialBT.read();
    }
    

    if(inChar != '\n'){
      Serial.print((char)inChar);

      inString += (char)inChar;//encola los caracteres recibidos

    }else{//si llegó el caracter de terminación
      
      Serial.print("Input string: ");
      Serial.println(inString);
      Serial.print("string length: ");
      Serial.println(inString.length());


      //obtiene el identificador
      charParamID = inString.charAt(0);
      
      Serial.println(charParamID);
      
      //obtiene el valor
      for(int i = 1; i < inString.length(); i++){
        valorParam += inString.charAt(i);
      }

      Serial.println(valorParam);

      //evalua el identificador y los parámetros enviados
      switchCaseParametros(charParamID, valorParam);
      
      //borra el contenido y lo prepara para recibir uno nuevo
      inString = "";
    
    }
  }

}