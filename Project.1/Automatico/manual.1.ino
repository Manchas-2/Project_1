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
char DATO = '0';
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

  // Lectura en grados Celsius
  float temp = sensors.getTempCByIndex(0);
  
  if (temp == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: No se pudo leer el sensor de temperatura!");
    delay(2000);
    return; // Salir del loop y esperar para intentar de nuevo
  }
   Serial.print("Temperatura sensor: ");
      Serial.print(temp);
      Serial.println("°C");

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
        digitalWrite(rest, HIGH);
        delay(1500);
        digitalWrite(rest, LOW);
        sensors.requestTemperatures(); // Se toma nuevamente la temperatura
        temp = sensors.getTempCByIndex(0);
        delay(2500);
      }

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
        digitalWrite(dis, HIGH);
        delay(1500);
        digitalWrite(dis, LOW);
        sensors.requestTemperatures(); // Se toma nuevamente la temperatura
        temp = sensors.getTempCByIndex(0);
        delay(2500);
      }

}
