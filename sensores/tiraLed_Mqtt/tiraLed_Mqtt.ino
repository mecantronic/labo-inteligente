/*
Curso de Arduino
Iluminacion gradual con LEDs
Descripcion
Controlaremos la intensidad de la ilumniacion con un potenciometro
Montaje
Potenciometro Verde               A0
Potencimetro Rojo                 A1
Potenciometro Azul                A2
Transistor Mosfet (de potencia) Azul     9
Transistor Mosfet (de potencia) Rojo     10
Transistor Mosfet (de potencia) Verde     11
by JAVacasM
*/
#include <WiFi.h> 
#include <PubSubClient.h> 
#define ssid "infiniem" // re
//#define ssid "Fibertel WiFi825 5.8Ghz" //
#define password  "12345678"// contraseña
//#define password  "004"// contraseña
//#define mqtt_server  "tcp://mqtt.eclipseprojects.io"//ip del raspi o del servidor mqtt
#define mqtt_server "192.168.2.112" //ip de donde corre el server mqtt

#define mqtt_user "" //opcional
#define mqtt_pass ""//opcional
#define root_topic_subscribe "red/set" // declaro topicos que suscribe 
#define root_topic_subscribe2 "blue/set" 
#define root_topic_subscribe3 "green/set" 
#define root_topic_publish "red/status"// declaro topicos que publica
#define root_topic_publish2 "blue/status"// declaro topicos que publica
#define root_topic_publish3 "green/status"// declaro topicos que publica



int iBrilloG = 0; // es el cable rojo 
int iBrilloR = 200; // es el cable azul
int iBrilloB = 240; // es el cable del verde
int instruccion;

WiFiClient esp2Client;
PubSubClient client(esp2Client);


/* ==== Defines - Constantes ==== */
#define PIN_POT_G 12
#define PIN_POT_R 14
#define PIN_POT_B 13

//#define PIN_PWM_G 25 // modifica red
//#define PIN_PWM_R 33// modifica blue
//#define PIN_PWM_B 32 // modifica green

//#define PIN_PWM_G 32 // modifica red
//#define PIN_PWM_R 25// modifica blue
//#define PIN_PWM_B 33 // modifica green


#define PIN_PWM_G 33 // modifica red
#define PIN_PWM_R 25// modifica blue
#define PIN_PWM_B 32 // modifica green

int iBrilloAnalogG;
int iBrilloAnalogR;
int iBrilloAnalogB;

int iBrilloAnalogGPREVIO = 0;
int iBrilloAnalogRPREVIO = 0;
int iBrilloAnalogBPREVIO = 0;

/* ==== END Defines ==== */

void miTarea2 (void *pvParameters){
  for(;;){
//boton();
    }}
  // CONECTAR MQTT
//
void reconnect() {
    while (!client.connected()) {
       Serial.print("Intentando conectar a Broker MQTT...");
    if (client.connect("ESP32 tira led")) {    
        Serial.println("connected");
    if(client.subscribe(root_topic_subscribe, 1));
         { Serial.println("subscripto");   } 
    if(client.subscribe(root_topic_subscribe2, 1));
         { Serial.println("subscripto2");   } 
     if(client.subscribe(root_topic_subscribe3, 1));
         { Serial.println("subscripto3");   } 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
            delay(5000);//esperar 5 segundos antes de volver a intentar
    }
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");// imprimo puntos hasta que se conecte
  }

  Serial.println("");
  Serial.println("WiFi connected");//imprimo datos de la red al conectarme
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


// FUNCION CALLBACK
//
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp="";
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();
  instruccion=messageTemp.toInt(); 
  if (String(topic) == root_topic_subscribe) {        //ponemos el topico al que estamos suscriptos
             
  analogWrite(PIN_PWM_R, instruccion);
     char cstr[16];
      itoa(instruccion, cstr, 10);//guardo la nueva posicion y la publico
    client.publish(root_topic_publish,cstr);
                 //llamo funcion onoff que tiene como parametro el string del mensaje
     }

    Serial.println();
  if (String(topic) == root_topic_subscribe2) {        //ponemos el topico al que estamos suscriptos
  analogWrite(PIN_PWM_B, instruccion);
  char cstr[16];
      itoa(instruccion, cstr, 10);
   client.publish(root_topic_publish2,cstr);//llamo funcion onoff que tiene como parametro el string del mensaje
     }

    Serial.println();
  if (String(topic) == root_topic_subscribe3) {        //ponemos el topico al que estamos suscriptos
     analogWrite(PIN_PWM_G, instruccion);  
     char cstr[16];
      itoa(instruccion, cstr, 10);
      client.publish(root_topic_publish3,cstr);//llamo funcion onoff que tiene como parametro el string del mensaje
     }
}





/* ==== Setup - Configuracion ==== */
void setup() {
// Nada que configurar aqui
pinMode(PIN_POT_G, INPUT);    
pinMode(PIN_POT_R, INPUT);    
pinMode(PIN_POT_B, INPUT); 
//   
pinMode(PIN_PWM_G, OUTPUT);    
pinMode(PIN_PWM_R, OUTPUT);    
pinMode(PIN_PWM_B, OUTPUT);    

xTaskCreate(miTarea2, "mi_tarea", 10000, NULL, 1, NULL);
 Serial.begin(115200);
  Serial.println("Iniciando");
  setup_wifi();
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
digitalWrite(PIN_PWM_G, iBrilloG);
digitalWrite(PIN_PWM_R, iBrilloR);
digitalWrite(PIN_PWM_B, iBrilloB);

delay(30);

}
/* ==== END Setup ==== */

/* ==== Loop - Codigo que se repite==== */
void loop() {

//   
        if (!client.connected()) {
    reconnect();           //if para mantenerme conectadoa mqtt
  }
 client.loop(); 
// 


//  int iPotG=analogRead(PIN_POT_G);
//  int iPotR=analogRead(PIN_POT_R);
//  int iPotB=analogRead(PIN_POT_B);
//  // Convertimos el valor del potenciometro (0-1023)
//  // a la escala de brillo (0-255)
//  
//  int iBrilloG=map(iPotG,0,1023,0,255);
//  int iBrilloR=map(iPotR,0,1023,0,255);
//  int iBrilloB=map(iPotB,0,1023,0,255);
  
//  digitalWrite(PIN_PWM_G, iBrilloG);
//  digitalWrite(PIN_PWM_R, iBrilloR);
//  digitalWrite(PIN_PWM_B, iBrilloB);


//    digitalWrite(PIN_PWM_G, 255);
//  digitalWrite(PIN_PWM_R, 0);
//  digitalWrite(PIN_PWM_B, 0);
     // Esperamos un tiempo a que se estabilice el brillo
}

void boton(){
  
  int iPotG=analogRead(PIN_POT_G);
     iBrilloAnalogG=map(iPotG,0,1023,0,255);
      Serial.print("Nuevo brillo green");
      Serial.println(iPotG);
//  if(iBrilloAnalogG != iBrilloAnalogGPREVIO){
//    digitalWrite(PIN_PWM_G, iBrilloAnalogG);
//        Serial.print("SET Nuevo brillo green");
//      Serial.println(iBrilloAnalogG);
//    iBrilloAnalogGPREVIO = iBrilloAnalogG ;
//  }

   int iPotB=analogRead(PIN_POT_B);
    iBrilloAnalogB=map(iPotB,0,1023,0,255);
    Serial.print("Nuevo brillo blue");
      Serial.println(iPotB);
//  if(iBrilloAnalogB != iBrilloAnalogBPREVIO){
//    digitalWrite(PIN_PWM_B, iBrilloAnalogB);
//            Serial.print(" SET Nuevo brillo blue");
//      Serial.println(iBrilloAnalogB);
//    iBrilloAnalogBPREVIO = iBrilloAnalogB ;
//  }

    int iPotR=analogRead(PIN_POT_R);
  iBrilloAnalogR=map(iPotR,0,1023,0,255);
        Serial.print("Nuevo brillo red");
      Serial.println(iPotR);
//  if(iBrilloAnalogR != iBrilloAnalogRPREVIO){
//    digitalWrite(PIN_PWM_R, iBrilloAnalogR);
//            Serial.print("SET Nuevo brillo red");
//      Serial.println(iBrilloAnalogR);
//    iBrilloAnalogRPREVIO = iBrilloAnalogR ;
//  }
 Serial.println();
  Serial.println();
  delay(5000);
}
