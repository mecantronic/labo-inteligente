#include <WiFi.h> 
#include <PubSubClient.h> 
#include <RBDdimmer.h>
#define AC_LOAD 5                                    //Pin "PWM" (salida)
int dimming;// Valor de intensidad
dimmerLamp acd(AC_LOAD,4);
int MIN_POWER  = 18;
int MAX_POWER  = 90;
int POWER_STEP  = 1;
int power  = 15;
int instruccion = 15;


//*******DEFINE*****
#define ssid "infiniem" // re
#define password  "12345678"// contraseña
//#define mqtt_server  "tcp://mqtt.eclipseprojects.io"//ip del raspi o del servidor mqtt
#define mqtt_server "192.168.2.112" //ip de donde corre el server mqtt

#define mqtt_user "" //opcional
#define mqtt_pass ""//opcional
#define root_topic_subscribe "dimmer/set" // declaro topicos que suscribe 
#define root_topic_publish "dimmer/status"// declaro topicos que publica

#define root_topic_subscribe2 "onoff/set" // declaro topicos que suscribe 
#define root_topic_publish2 "onoff/status"// declaro topicos que publica
//#define LED_SENAL LED_BUILTIN
#define PIN_LLAVE 19


WiFiClient espClient;
PubSubClient client(espClient);



void zero_crosss_int()  // Esta interrupción se ejecuta en el cruce por cero, cada 10mS para 50Hz
  {
  //Cálculo para 50Hz: 1/50=20mS
  //Entonces medio ciclo= 10mS = 10000uS
  //(10000uS - 10uS) / 128 = 75 (aproximadamente), 10uS propagación 
  delayMicroseconds(75*dimming);    
  digitalWrite(AC_LOAD, HIGH);   
  delayMicroseconds(10);         
  digitalWrite(AC_LOAD, LOW); 


  }

  // CONECTAR MQTT
//
void reconnect() {
    while (!client.connected()) {
       Serial.print("Intentando conectar a Broker MQTT...");
    if (client.connect("ESP8266 Dimmer 1")) {    
        Serial.println("connected");
    if(client.subscribe(root_topic_subscribe, 1));
         { Serial.println("subscripto");   } 
    if(client.subscribe(root_topic_subscribe2, 1));
         { Serial.println("subscripto2");   } 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
            delay(5000);//esperar 5 segundos antes de volver a intentar
    }
  }
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
  if (String(topic) == root_topic_subscribe) {        //ponemos el topico al que estamos suscriptos
   instruccion=messageTemp.toInt();                             //llamo funcion onoff que tiene como parametro el string del mensaje
     }

    Serial.println();
  if (String(topic) == root_topic_subscribe2) {        //ponemos el topico al que estamos suscriptos
    onoff(messageTemp);                              //llamo funcion onoff que tiene como parametro el string del mensaje
     }

}

void dimmerUp(){

      
     do{ 
      acd.setPower(power);
      Serial.print("lampValue -> ");
      Serial.print(power);
      power+=POWER_STEP;
      Serial.println("%");
      delay(100);
    }while(power<=instruccion);
    power = instruccion;
}


void dimmerDown(){

      
     do{ 
      acd.setPower(power);
      Serial.print("lampValue -> ");
      Serial.print(power);
      power-=POWER_STEP;
      Serial.println("%");
      delay(300);
    }while(power>=instruccion);
    power = instruccion;
}


void core2 (void *pvParameters){
  for(;;){
if(instruccion < power)
{dimmerDown();}
if(instruccion > power)
{dimmerUp();}
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

void setup()
  {
  Serial.begin(115200);
  Serial.println("Iniciando");
    xTaskCreate(core2, "core_2", 10000, NULL, 1, NULL);
  setup_wifi();
    client.setServer(mqtt_server,1883);
  client.setCallback(callback);
  client.setKeepAlive(450);
  pinMode(AC_LOAD, OUTPUT);     
  acd.begin(NORMAL_MODE, ON);
  acd.setPower(power);
  pinMode(PIN_LLAVE, OUTPUT);
    

  }


void loop() 
  {
    if (!client.connected()) {
    reconnect();           //if para mantenerme conectadoa mqtt
  }
 client.loop(); 


  }

void onoff(const String& messageTemp){
 Serial.println("Estado del enchufe: ");
    if(messageTemp == "on"){
      prender();
    }
    else if(messageTemp == "off"){
      apagar();
    }  }

// FUNCION APAGAR
//Apaga la salida, apaga led señal y publica estado 
void apagar(){
  Serial.println("off");
  digitalWrite(PIN_LLAVE, LOW);
  client.publish(root_topic_publish2,"off");
}

// FUNCION PRENDER
// Prende la salida, prende led señal y publica estado
void prender (){
  Serial.println("on");
  digitalWrite(PIN_LLAVE, HIGH);
  client.publish(root_topic_publish2,"on");
    }
  
