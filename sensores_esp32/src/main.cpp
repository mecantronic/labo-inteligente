#include <toggleLED.h>
#include <comunicacion.h>
#include <EEPROM.h>



//void toggleLED(void);

void setup() {
  // put your main code here, to run repeatedly:
  //delay(10000);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Sensores sensorTemp;  // Create an object

  // Access attributes and set values
  sensorTemp.value = 25;
  sensorTemp.SSID = "infiniem";  
  sensorTemp.Password = "12345678";  
  

  // Print attribute values

  Serial.println(sensorTemp.value);
  Serial.println(sensorTemp.SSID);
  Serial.println(sensorTemp.Password);
    
}

void loop() {
  // put your main code here, to run repeatedly:
  toggleLED();
  delay(1000);
  //printScreen();
  pepe();
  delay(500);//linea agregada por Martín
}