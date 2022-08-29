
#include <WiFi.h> 
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "infiniem"
#define WLAN_PASS       "12345678"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "tomabregu"
#define AIO_KEY         "aio_UtiP73L2fWdYG0Z3AunzjTFKdOvY"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish tension = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/tension");
Adafruit_MQTT_Publish corriente = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/corriente");
// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();


unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

const int tensionPin = 35;   // seleccionar la entrada para el sensor
int sensorValue;         // variable que almacena el valor raw (0 a 1023)
int value;            // variable que almacena el voltaje (0.0 a 25.0)
float voltage = 0.0;
int currentValue;
float ic =0;

int corrientePin  = 32; 

int SAMPLESNUMBER = 100;

float SENSIBILITY = 0.100; // Modelo 20A

float corrienteSum;
int sensor = 0;


float getCorriente()
{
   float corrienteSum = 0.0;

  for (int i = 0; i < 1000; i++) {
  currentValue = analogRead(corrientePin);
  sensor += currentValue;
  }
  sensor = sensor/1000;
//     Serial.print("Analog read: ");
//    Serial.println(sensor);
  ic = sensor * 3.3 / 4096;
  sensor = 0;
  Serial.print("tension medida: ");
Serial.println(ic);
//corrienteSum = ((ic - 2.578) / 0.118)  ;
 corrienteSum = ((ic - 2.44) / 0.133)  ;

 Serial.println();
 Serial.println();
   return(corrienteSum);
}



float v2(){
  
 for (int i = 0; i < 1000; i++)
   {
     sensorValue = analogRead(tensionPin);
     value += sensorValue;
  
  }
   value = value/1000;
    Serial.print("Valor medido:"); 
  Serial.println(value); 
    float value2 = ((map(value, 0, 4095, 0, 16500))/1000.00);
    if(value2 < 14.1)
    {    Serial.println("if 1 :"); 

      value2 += 0.45;}
    if( value2 > 14.5){
      Serial.println("if 2 :"); 
      value2 -= 0.45;
    }
        Serial.print("tension: "); 
  Serial.println(value2); 
  Serial.println(); 
  Serial.println(); 
//     Serial.print("Valor tension ref 3.3: "); 
//  Serial.println(value2); 
value = 0.0;
   return value2;
}

void setup() {
  Serial.begin(115200);
  delay(10);

pinMode(corrientePin, INPUT);
pinMode(tensionPin, INPUT);  
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
//  mqtt.subscribe(&onoffbutton);
}

uint32_t x=5;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

//  Adafruit_MQTT_Subscribe *subscription;
//  while ((subscription = mqtt.readSubscription(5000))) {
//    if (subscription == &onoffbutton) {
//      Serial.print(F("Got: "));
//      Serial.println((char *)onoffbutton.lastread);
//    }
//  }

  // Now we can publish stuff!
//  Serial.print(F("\nSending photocell val "));
//  Serial.print(x);
//  Serial.print("...");
//  if (! photocell.publish(x++)) {
//    Serial.println(F("Failed"));
//  } else {
//    Serial.println(F("OK!"));
//  }




   tiempo2 = millis();
//  if(tiempo2 > (tiempo1 + 1800000)){  //Si ha pasado 1 segundo ejecuta el IF
     if(tiempo2 > (tiempo1 + 60000)){  //Si ha pasado 1 segundo ejecuta el IF
    tiempo1 = millis(); //Actualiza el tiempo actual
    float current = getCorriente();
//    if( current < 0){
//      current = 0.0;
//    }
    Serial.print("Corriente medida: ");
    Serial.println(current);
    corriente.publish(current);
//    char cstr[16];
//    itoa(current, cstr, 10);//guardo la nueva posicion y la publico
//    client.publish(root_topic_publish,cstr);
  delay(5000);
  
   tension.publish(v2());
}
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
