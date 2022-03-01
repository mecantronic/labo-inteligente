#include <toggleLED.h>

#include <EEPROM.h>



//void toggleLED(void);

void setup() {
  // put your main code here, to run repeatedly:
  //delay(10000);
  //pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  toggleLED();
  delay(1000);
  //printScreen();
  pepe();
}