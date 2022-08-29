const int sensorPin = 15;   // seleccionar la entrada para el sensor
int sensorValue;         // variable que almacena el valor raw (0 a 1023)
int value;            // variable que almacena el voltaje (0.0 a 25.0)
float voltage = 0.0;
void setup() {
   Serial.begin(115200);
}

void loop() {
  float bla = v2();
  Serial.print("Valor tension: ");   
  Serial.println(bla);                       
   delay(3000);
}


float v2(){
  
 for (int i = 0; i < 1000; i++)
   {
     sensorValue = analogRead(sensorPin);
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
//     Serial.print("Valor tension ref 3.3: "); 
//  Serial.println(value2); 
value = 0.0;
   return value2;
}
