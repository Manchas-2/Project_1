#include "BluetoothSerial.h"
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 4;
const int rest = 14;  
const int dis = 27;
BluetoothSerial SerialBT;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float mx = 32.1;
char DATO ;
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
  delay(1000);
  // Modo automático
  // Leer la temperatura
  Serial.println("Lectura");
  sensors.requestTemperatures();
  
  // Lectura en grados Celsius
  float temp = sensors.getTempCByIndex(0); 
      Serial.print("Sistema remoto");
      Serial.print("Temperatura= ");
      Serial.print(temp);
      Serial.println(" °C"); 
      SerialBT.print(temp);
      if (SerialBT.available()) {
        DATO = SerialBT.read();
      }
        if (DATO == '0') { // Activado 
          while (temp < mx&& DATO != '1') {
            // Impresión en Monitor serial
            Serial.print("Temperatura= ");
            Serial.print(temp);
            Serial.println(" C°");
            Serial.println("Calentando...");
            // Impresión en Bluetooth
          
            SerialBT.print(temp);
          
            digitalWrite(rest, HIGH);
            delay(1500);
            digitalWrite(rest, LOW);
            sensors.requestTemperatures(); // Se toma nuevamente la temperatura
            DATO = SerialBT.read();
             temp = sensors.getTempCByIndex(0);
            delay(2500);
            if (DATO == '1') { // Apagado
          Serial.println("Sistema Apagado");
           }
         }
          while (temp >= mx && DATO != '1') {
            // Impresión en Monitor serial
            Serial.print("Temperatura= ");
            Serial.print(temp);
            Serial.println(" C°");
            Serial.println("Listo.");
            // Impresión en Bluetooth
            
            SerialBT.print(temp);
            digitalWrite(dis, HIGH);
            delay(1500);
            digitalWrite(dis, LOW);
            sensors.requestTemperatures(); // Se toma nuevamente la temperatura
            temp = sensors.getTempCByIndex(0);
            DATO = SerialBT.read();
            delay(2500);
            if (DATO == '1') { // Apagado
          Serial.println("Sistema Apagado");
          }
        }
        }
       else if (DATO == '1') { // Apagado
          Serial.println("Sistema Apagado");
        } 
        else {
          Serial.println("Sistema sin orden");
        }
      
}
