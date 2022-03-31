//#include <toggleLED.h>
#include <comunicacion.h>
#include <EEPROM.h>
#include <DHT.h>

#define DHTPIN 33    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);// Initialize DHT sensor.



Network conexion;
Sensores sensorTemp;  // Create an object
Parametros parametros;
//Sensores sensor1;


//void toggleLED(void);

void setup() {
  // put your main code here, to run repeatedly:
  //delay(10000);
  Serial.begin(115200);
  conexion.setup_bluetooth();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  
  

  // Access attributes and set values
  sensorTemp.value = 25.0;
  //sensorTemp.SSID = "infiniem";
  //sensorTemp.SSID = "milton";    
  sensorTemp.SSID = "wifi01-ei";    
  //sensorTemp.Password = "12345678";
  //sensorTemp.Password = "paternal";
  sensorTemp.Password = "Ax32MnF1975-ReB";
  sensorTemp.descriptor = "Mide temperatura, humedad y presion";
  sensorTemp.url_broker = "broker.hivemq.com";
  sensorTemp.id = "sensorTemp_0001";
  sensorTemp.topic = "labo_inteligente/temperatura/"+ sensorTemp.id;  

  conexion.ssid = sensorTemp.SSID;
  conexion.password = sensorTemp.Password;

  //parametros.ssid = "wifi01-ei";
  //parametros.password = "Ax32MnF1975-ReB";
  
 
  // Print attribute values
  Serial.println(sensorTemp.value);
  Serial.println(sensorTemp.SSID);
  Serial.println(sensorTemp.Password);

  conexion.setup_wifi(sensorTemp.SSID, sensorTemp.Password);
  conexion.setup_mqtt(sensorTemp.url_broker, sensorTemp.topic);
    
}

void loop() {
  // put your main code here, to run repeatedly:

  bool resultadoPub = 0;
  //int cantFallasMQTT = 0;

  conexion.loop();

  conexion.cambioDeParametros();

  
  delay(3000);
  sensorTemp.value = dht.readTemperature();
  Serial.println(dht.readTemperature());

  resultadoPub = conexion.publicarData(sensorTemp.value);
  if(resultadoPub == 0){//si falla la publicación
	conexion.flagConexionOK = 0;
  }
  
  Serial.print("Resultado de la publicación: ");
  Serial.println(resultadoPub);
  Serial.print("Cantidad de fallas de publicación MQTT: ");
  Serial.println(conexion.cantDeFallasMQTT);
  

	if(conexion.flagConexionOK == 0){
		
		Serial.println("Intentando recuperar la conexión");
		//si alguna conexión se perdió, la reestablece
		conexion.comprobarConexion(conexion.ssid,conexion.password,sensorTemp.url_broker,sensorTemp.topic);
		
		if(conexion.flagConexionOK){//si la recuperó
			Serial.print("Se ha recuperado la conexión. flagConexionOK = ");
			Serial.println(conexion.flagConexionOK);
			conexion.publicarData(sensorTemp.value);// y publica
			Serial.println("publica ");
  			conexion.serialBTprintln("publica");
			//contadorReconexionExitosa++;
		}else{//si no la recuperó
		
			Serial.print("[PROBLEMAS] No se ha recuperado la conexión. flagConexionOK = ");
			Serial.println(conexion.flagConexionOK);
			//contadorReconexionFallida++;
		}


	}

}