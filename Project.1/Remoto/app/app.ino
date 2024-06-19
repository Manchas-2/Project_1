//DECLARAAR VAIRABLES 
#include "BluetoothSerial.h"
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 4;// PIN sensor
const int rest = 14;  //PIN resistencia de calefacción
const int dis = 27;   //PIN disipador 
BluetoothSerial SerialBT;

//Declarar funciones del sensor
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

float mx = 33.8; //Temperatura tope 
char DATO ;//DATO DE APLICACIÓN
int i;
int n;

void setup() {
 // Ajustar la velocidad para el monitor serie
  Serial.begin(115200);
  sensors.begin();
 SerialBT.begin("ESP32test"); //Bluetooth device name

 // Configurar pines como salidas
  pinMode(rest, OUTPUT);
  pinMode(dis, OUTPUT);
}

void loop() {
 // Leer la temperatura
  Serial.println("Lectura");
  sensors.requestTemperatures();
 delay(1000);
 // Lectura en grados Celsius
  float temp = sensors.getTempCByIndex(0); 

 //Corroborar que la lectura del sensor  
  if (temp == DEVICE_DISCONNECTED_C) {     
   Serial.println("Error: No se pudo leer el sensor de temperatura!");
   delay(2000);
   return; // Salir del loop y esperar para intentar de nuevo
  }

 //Impreción de datos Serial y BT
  Serial.print("Sistema remoto");
  Serial.print("Temperatura= ");
  Serial.print(temp);
  Serial.println(" °C"); 
  SerialBT.print(temp);
 
 //Recibir datos BT y alcenarlos
 if (SerialBT.available()) {
  DATO = SerialBT.read();
 }
//Clasificar los datos 
 if (DATO == '0') { // Activado
  while (i==n && DATO != '1'){//Blucle semi-infinito

  //Ciclo de calefacción
   while (temp < mx&& DATO != '1') {
     // Impresión en Monitor serial
      Serial.print("Temperatura= ");
      Serial.print(temp);
      Serial.println(" C°");
      Serial.println("Calentando...");       
     // Impresión en Bluetooth
       SerialBT.println(temp);
     //Funcionamiento de calefacción 
      digitalWrite(rest, HIGH);
      delay(18000);
      digitalWrite(rest, LOW);
     //Toma de temperatura
      sensors.requestTemperatures(); 
      temp = sensors.getTempCByIndex(0);
     //Validar orden BT
      DATO = SerialBT.read();
      if (DATO == '1') { // Apagado
      Serial.println("Sistema Apagado");
      delay(8000);
       }
   }
  //Ciclo de disipación
   while (temp >= mx && DATO != '1') {
     // Impresión en Monitor serial
      Serial.print("Temperatura= ");
      Serial.print(temp);
      Serial.println(" C°");
      Serial.println("Listo.");
     // Impresión en Bluetooth
      SerialBT.println(temp);
     //Funcionamiento del dispador
      digitalWrite(dis, HIGH);
      delay(4000);
      digitalWrite(dis, LOW);
     //Toma de temperatura
      sensors.requestTemperatures(); 
      temp = sensors.getTempCByIndex(0);
     //Validar orden BT
      DATO = SerialBT.read();
      if (DATO == '1') { // Apagado
      Serial.println("Sistema Apagado");
      delay(1000);          
      }
   }
   //Validar orden BT
    DATO = SerialBT.read();
    if (DATO == '1') { // Apagado
     Serial.println("Sistema Apagado");
    }
   }      
 }             
 else if (DATO == '1') { // Apagado
  Serial.println("Sistema Apagado");
 } 
 else { //Sin señal 
  Serial.println("Sistema sin orden");
 }      
}
