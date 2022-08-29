//*****Librerias*****

#include <WiFi.h> // libreria wifi
#include <PubSubClient.h> //libreria mqtt
//#include <EEPROM.h>
#include <Preferences.h> 


//*******DEFINE*****
#define ssid "infiniem" // re
//#define ssid "Fibertel WiFi825 5.8Ghz" //
#define password  "12345678"// contraseña
//#define password  "004"// contraseña
//#define mqtt_server  "tcp://mqtt.eclipseprojects.io"//ip del raspi o del servidor mqtt
#define mqtt_server "192.168.2.112" //ip de donde corre el server mqtt

#define mqtt_user "" //opcional
#define mqtt_pass ""//opcional
#define root_topic_subscribe "cortinas/set" // declaro topicos que suscribe 
#define root_topic_publish "cortinas/status"// declaro topicos que publica
#define root_topic_subscribe2 "cortinas/setting/pos"// declaro topicos que publica
#define root_topic_subscribe3 "cortinas/setting/vel"// declaro topicos que publica
#define dirPin 16
#define stepPin 5
//#define a int
//#define posicion int
//#define posicion_nueva int
//#define Pasos_tot int

#define botonUp 3
#define botonDown 21

//*****Variables globales****

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int Pasos_tot;
int posicion;
int a;
int posicion_nueva;
int pasosTotales= 5000;
Preferences preferences;
int velocidad = 1000;
int velocidadMotor1 = 900;
int velocidadMotor2 = 1000;
int velocidadMotor3 = 1100;

void miTarea2 (void *pvParameters){
  for(;;){

if(digitalRead(botonUp) == HIGH && digitalRead(botonDown) == LOW){
   digitalWrite(dirPin,LOW);
   a= 50;
   posicion -= a ;
   mover();

   
    Serial.print("Posicion modificada por boton 1 ");
   Serial.println(posicion);
}
if(digitalRead(botonDown) == HIGH && digitalRead(botonUp) == LOW){
  
   digitalWrite(dirPin,HIGH);
    a= 50;
   posicion +=  a;
   mover();
   
   
   Serial.print("Posicion modificada por boton 2 ");
   Serial.println(posicion);
}
    
}}

//****FUNCIONES****

void setup() {
  pinMode(dirPin, OUTPUT);
   pinMode(stepPin, OUTPUT);
  Serial.begin(115200);
  xTaskCreate(miTarea2, "mi_tarea", 10000, NULL, 1, NULL);
  setup_wifi();
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
  //setup_stepper();
  
   preferences.begin("myfile", false);

//   int posActual = 5;
   
//      preferences.putUInt("posActual", posActual);
    posicion = preferences.getUInt("posActual", 0); // get boardId or if key doesn't exist set variable to 0
    Serial.print("Read Id = ");
    Serial.println(posicion);

}

void loop() {
  
if (!client.connected()) {
    reconnect();           //if para mantenerme conectadoa mqtt
  }
 client.loop();  // se ejecuta el metodo para que los mensajes lleguen.
  }



//******FUNCIONES ESPECIALES************

// CONECTAR A WIFI
//
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

// FUNCION setup_stepper
//Recorro todo el trayecto, cuento pasos totales, y vuelvo a posicion inicial.
//void setup_stepper(){
//   Serial.println("Colocar cortinar en posicion 0");
//   delay(3000);
//   int Final_carrera=1000;//ver como era, tenia el 10 puesto
//   int count=0;
//    digitalWrite(dirPin,HIGH);
//   while(Final_carrera !=0)
//      { //para ir en direccion contraria low 
//         digitalWrite(stepPin,HIGH);
//         delayMicroseconds(10); 
//         digitalWrite(stepPin,LOW); 
//         delayMicroseconds(10); 
//         count++;
//         Final_carrera=Final_carrera-1;
//      }
//   Serial.println("La cortina llego a la posicion final");
//      Pasos_tot=count;
//      posicion=Pasos_tot;
//   delay(3000);
//   digitalWrite(dirPin,LOW);
//   while(posicion !=0)
//   {     //para ir en direccion contraria low 
//         digitalWrite(stepPin,HIGH);
//         delayMicroseconds(10); 
//         digitalWrite(stepPin,LOW); 
//         delayMicroseconds(10);
//         posicion--;
//       }
//       Pasos_tot = 5000;
//       posicion =0;
//   Serial.println("Cortina seteada");
//   Serial.print("Posicion actual:");
//   Serial.println(posicion);
//   Serial.print("Cantidad de pasos totales");
//   Serial.println(Pasos_tot);     
//}

//FUNCION MENSAJE
//Determino si tengo que avanzar o retroceder y publico nueva posicion. (de 1 a 100)
void Mensaje(const String& messageTemp){
      a=0;//pasos a realizar entre posicion actual y nueva
      int instruccion;
      instruccion=messageTemp.toInt();
      posicion_nueva=(instruccion*pasosTotales)/100;//transformo mensaje de 1-100 en pasos
      Serial.print("pasos totales: ");
      Serial.println(Pasos_tot);
      Serial.print("posicion nueva: ");
      Serial.println(posicion_nueva);
      if (posicion > posicion_nueva)
         
          {Serial.println("abriendo");
            a=posicion-posicion_nueva;//si estoy mas adelante debo retroceder
           digitalWrite(dirPin,LOW);
           mover();
            }
      else if(posicion <= posicion_nueva)
      {Serial.println("cerrando");
        a=posicion_nueva-posicion;//si estoy mas atras debo avanzar
        digitalWrite(dirPin,HIGH);
        mover();}
      preferences.putUInt("posActual", posicion_nueva);
      Serial.println(posicion_nueva);
      posicion=posicion_nueva;
      char cstr[16];
      itoa(instruccion, cstr, 10);//guardo la nueva posicion y la publico
      client.publish(root_topic_publish,cstr);
      }
// FUNCION MOVER
// 
void mover() 
{       
    Serial.print("posicion previa: ");
     Serial.println(posicion);
  while(a!=0){
     Serial.print("pasos restantes: ");
     Serial.println(a);
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(velocidad); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(velocidad);
        a--;}
}        

// CONECTAR MQTT
//
void reconnect() {
    while (!client.connected()) {
       Serial.print("Intentando conectar a Broker MQTT...");
    if (client.connect("ESP8266 Cortinas 1")) {    
        Serial.println("connected");
    if(client.subscribe(root_topic_subscribe))
         { Serial.println("subscripto1");   } 
    if(client.subscribe(root_topic_subscribe2))
         { Serial.println("subscripto2");   } 
    if(client.subscribe(root_topic_subscribe3))
         { Serial.println("subscripto3");   }      
                                        
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
    
  String topico = String(topic);
  if(String(topic) == root_topic_subscribe2){
    Serial.print("topico setting: ");
  Serial.println(topic);
  posicion = messageTemp.toInt();
    }

 if(String(topic) == root_topic_subscribe3)
  {Serial.print("topico setting: ");
  Serial.println(topic);
    if( messageTemp.equals("1"))
    {velocidad= velocidadMotor1;}
       if( messageTemp.equals("2"))
    {velocidad= velocidadMotor2;}
         if( messageTemp.equals("3"))
    {velocidad= velocidadMotor3;}
    }

  Serial.println();
  if (String(topic) == root_topic_subscribe) {        //ponemos el topico al que estamos suscriptos
    Mensaje(messageTemp);                              //llamo funcion onoff que tiene como parametro el string del mensaje
     }

}
