/*
  Lidar VL53L0X

  Uso de rotor y envio de datos mediante bluetooth

  Programa realizado para Arduino Uno con Prototype shield v.5  

  created during 2021
  by Juanfer Mata
*/

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//definicion de pins
const int motorPin1 = 8;    // 28BYJ48 In1
const int motorPin2 = 9;    // 28BYJ48 In2
const int motorPin3 = 10;   // 28BYJ48 In3
const int motorPin4 = 11;   // 28BYJ48 In4
const int pinHall = A0; //pin magnético
                   
//definicion variables
int motorSpeed = 1200;   //variable para fijar la velocidad
int stepCounter = 0;     // contador para los pasos
int stepsPerRev = 5125;  // pasos para media vuelta del gris
int angulo = 0;          //angulo al inicio por progamar un mapeo
float pasos_mayor;

//tablas con la secuencia de encendido (descomentar la que necesiteis)
//secuencia 1-fase
//const int numSteps = 4;
//const int stepsLookup[4] = { B1000, B0100, B0010, B0001 };

//secuencia 2-fases
//const int numSteps = 4;
//const int stepsLookup[4] = { B1100, B0110, B0011, B1001 };

//secuencia media fase
const int numSteps = 8;
const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };

//===================================================================================

// declarar puertos del BT
 SoftwareSerial Puerto_Blutus(6, 7);

//sensor
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup(){
  //motor
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(pinHall, INPUT);
  
//sensor
  Serial.begin(9600);
  Puerto_Blutus.begin(115200);
   // Iniciar sensor
  Serial.println("VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Error al iniciar VL53L0X"));
    while(1);
  }
   uint32_t setMeasurementTimingBudgetMicroSeconds (200000);
   magnetico();
   delay(200);
}

void loop(){
  VL53L0X_RangingMeasurementData_t measure; 
  for (int i = 0; i < stepsPerRev *2; i++){ //multiplicamos por 2 para vuelta completa
    angulo = i/100;
    clockwise();
    delayMicroseconds(motorSpeed);
    //===================================================================================
    if (i % 100 == 0) {
      //Medición
      //Serial.print("Leyendo sensor ");
      lox.rangingTest(&measure, false); // si se pasa true como parametro, muestra por puerto serie datos de debug
      int mapa = map(angulo, 0, 102, 0, 360);
      /*if (measure.RangeStatus != 4){
        Serial.print("Distancia (mm): ");
        Serial.print(measure.RangeMilliMeter);
        Serial.print("  Ángulo: ");
        Serial.print(angulo);
        Serial.print("  Grados: ");
        Serial.println(mapa);
      }else{
        Serial.print("Fuera de rango ");
        Serial.print("  Ángulo: ");
        Serial.print(angulo);
        Serial.print("  Grados: ");
        Serial.println(mapa);
      }*/
     String esp = " ";
     String com_pack = "";
     //Imprimir en el serie
     Serial.print(com_pack);
     //Distancia puerto serie
     if (measure.RangeMilliMeter < 1000) Serial.print(esp);
     if (measure.RangeMilliMeter < 100) Serial.print(esp);
     if (measure.RangeMilliMeter < 10) Serial.print(esp);
     Serial.print(measure.RangeMilliMeter);
     //Ángulo puerto serie
     /*if (mapa < 100) Serial.print(esp);
     if (mapa < 10) Serial.print(esp);
     Serial.print(mapa);
     Serial.print(esp);*/
     if (i < 10000) Serial.print(esp);
     if (i < 1000) Serial.print(esp);
     if (i < 100) Serial.print(esp);
     if (i < 10) Serial.print(esp);
     Serial.println(i);

     
     //Imprimir en el puerto BT
     Puerto_Blutus.print(com_pack);
     //Distancia bt
     if (measure.RangeMilliMeter < 1000) Puerto_Blutus.print(esp);
     if (measure.RangeMilliMeter < 100) Puerto_Blutus.print(esp);
     if (measure.RangeMilliMeter < 10) Puerto_Blutus.print(esp);
     Puerto_Blutus.print(measure.RangeMilliMeter);
     //Ángulo bt
     if (i < 10000) Puerto_Blutus.print(esp);
     if (i < 1000) Puerto_Blutus.print(esp);
     if (i < 100) Puerto_Blutus.print(esp);
     if (i < 10) Puerto_Blutus.print(esp);
     Puerto_Blutus.println(i);
    
    } 
     //Serial.print(measure.RangeMilliMeter);
  }
//  while(true);
}


void magnetico(){
  int j = 0;
  float almacenaje = 150000;
  pasos_mayor = 0;
  Serial.println ("Detección magnética: ");
for (j = 0; j < stepsPerRev *2; j++){
  clockwise();
  delayMicroseconds(motorSpeed);
  if (j % 100 == 0) {
    long measure = 0;
    for(int i = 0; i < 10; i++){
        int value = 
        measure += analogRead(pinHall);
      }
    measure /= 10;
  
  float outputV = measure * 5000.0 / 1023;
  //interpolacion a densidad de flujo magnético
  float magneticFlux =  outputV * 53.33 - 133.3;
  if (almacenaje > magneticFlux){
      almacenaje = magneticFlux;
      pasos_mayor = j;
      } 
  Serial.print(magneticFlux);
  Serial.print("   Pasos: ");
  Serial.println(pasos_mayor);
  delay(100);
    }
  }
  Serial.print("ángulo: ");
  Serial.println(pasos_mayor);
  for (int i = 0; i < pasos_mayor; i++){
  clockwise();
  delayMicroseconds(motorSpeed);
  }
}

void clockwise(){
  stepCounter++;
  if (stepCounter >= numSteps) stepCounter = 0;
  setOutput(stepCounter);
}

void anticlockwise(){
  stepCounter--;
  if (stepCounter < 0) stepCounter = numSteps - 1;
  setOutput(stepCounter);
}

void setOutput(int step){
  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
}
