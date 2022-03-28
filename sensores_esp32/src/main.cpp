//#include <toggleLED.h>
#include <comunicacion.h>
#include <EEPROM.h>
#include <DHT.h>

#define DHTPIN 33    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);// Initialize DHT sensor.


Network conexion;
Sensores sensorTemp;  // Create an object
//void toggleLED(void);

void setup() {
  // put your main code here, to run repeatedly:
  //delay(10000);
  Serial.begin(115200);
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
  
  // Print attribute values
  Serial.println(sensorTemp.value);
  Serial.println(sensorTemp.SSID);
  Serial.println(sensorTemp.Password);

  conexion.setup_wifi(sensorTemp.SSID, sensorTemp.Password);
  conexion.setup_mqtt(sensorTemp.url_broker, sensorTemp.topic);
    
}

void loop() {
  // put your main code here, to run repeatedly:

  conexion.loop();
  delay(3000);
  sensorTemp.value = dht.readTemperature();
  Serial.println(dht.readTemperature());

  conexion.publicarData(sensorTemp.value);
  Serial.println("publica ");
  /*
  toggleLED();
  delay(1000);
  //printScreen();
  pepe();
  delay(500);//linea agregada por Martín
  */



}