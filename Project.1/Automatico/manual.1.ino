//DECLARAR LIBRERIAS 
#include "BluetoothSerial.h"
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 4; //PIN sensor
const int rest = 14; // PIN resistencia de calefacción
const int dis = 27;  // PIN disipador 
BluetoothSerial SerialBT; 

//Decarar funciones del sensor 
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

float mx = 33.8; // temperatra tope de calefacción

void setup() {
 Serial.begin(115200); // Ajustar la velocidad para el monitor serie
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

 // Lectura en grados Celsius
  float temp = sensors.getTempCByIndex(0);

 //Corroborar que la lectura del sensor  
  if (temp == DEVICE_DISCONNECTED_C) {     
   Serial.println("Error: No se pudo leer el sensor de temperatura!");
   delay(2000);
   return; // Salir del loop y esperar para intentar de nuevo
 }

 //impreción de datos 
  Serial.print("Temperatura sensor: ");
  Serial.print(temp);
  Serial.println("°C");

 //Ciclo de calefacción 
  while (temp < mx) {
   // Impresión en Monitor serial
    Serial.print("Temperatura= ");
    Serial.print(temp);
    Serial.println(" C°");
    Serial.println("Calentando...");
   // Impresión en Bluetooth
    SerialBT.print("Temperatura= ");
    SerialBT.print(temp);
    SerialBT.println(" C°");
    SerialBT.println("Calentando...");
   // Funcionamiento de la calefacción
    digitalWrite(rest, HIGH);
    delay(16000);
    digitalWrite(rest, LOW);
   //nueva lesctura de temperatura
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0);
    delay(4000);//Tiempo para estabilizar la temperatura 
  } 
 //Ciclo de disipación 
  while (temp >= mx) {
   // Impresión en Monitor serial
    Serial.print("Temperatura= ");
    Serial.print(temp);
    Serial.println(" C°");
    Serial.println("Listo.");
   // Impresión en Bluetooth
    SerialBT.print("Temperatura= ");
    SerialBT.print(temp);
    SerialBT.println(" C°");
    SerialBT.println("Listo.");
   //Funcionamiento del dispador
    digitalWrite(dis, HIGH);
    delay(5000);
    digitalWrite(dis, LOW);
   //Toma de temperatura
    sensors.requestTemperatures(); 
    temp = sensors.getTempCByIndex(0);
        delay(1500);
  }
}
