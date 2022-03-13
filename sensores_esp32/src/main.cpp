//#include <toggleLED.h>
#include <comunicacion.h>
#include <EEPROM.h>


Network conexion;
Sensores sensorTemp;  // Create an object
//void toggleLED(void);

void setup() {
  // put your main code here, to run repeatedly:
  //delay(10000);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  

  // Access attributes and set values
  sensorTemp.value = 25.0;
  //sensorTemp.SSID = "infiniem";
  sensorTemp.SSID = "milton";    
  //sensorTemp.Password = "12345678";
  sensorTemp.Password = "paternal";
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
  conexion.publicarData((long)sensorTemp.value);
  Serial.println("publica ");
  /*
  toggleLED();
  delay(1000);
  //printScreen();
  pepe();
  delay(500);//linea agregada por Martín
  */



}