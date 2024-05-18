#include <SPI.h>

#include <Wire.h>

#include <SparkFun_ADXL345.h>


ADXL345 adxl = ADXL345 ();

void setup(){

Serial.begin (9600);

Serial.println ("Iniciar");

Serial.println ();

adxl.powerOn ();

adxl.setRangeSetting (16); // Defina o intervalo, valores 2, 4, 8 ou 16

}


void loop(){

// leia os valores e imprima-os

int x, y, z;

adxl.readAccel (& x, & y, & z);

Serial.print (x);

Serial.print (",");

Serial.print (y);

Serial.print (",");

Serial.println (z);

delay(500);

// normal values when not being touched "0, 4, 33" "x, y, z"

if (x < -5){
  Serial.println(" - front - ");
}

if (x > 5){
  Serial.println(" - back - ");
}

if (y < -20){
  Serial.println(" - right - ");
}

if (y > 20){
  Serial.println(" - left - ");
}

}