#include <CapacitiveSensor.h>
// accelerometer code ---
#include <SPI.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>
// ---
#define BUTTON 2
#define echoPin 7
#define trigPin 8
int touch = 0;

int x = 4;
int y = 5;
int z = 6;
long ellapsed;
float proximity;
long buttonRandomNumber;
CapacitiveSensor cs_4_3 = CapacitiveSensor(4, 3);  // 10M resistor between pins 7 & 6, pin 6 is sensor pin, add a wire and or foil if desired
long duration;

// accelerometer code ---
ADXL345 adxl = ADXL345 ();
// ---
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  randomSeed(100);
  cs_4_3.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turn off autocalibrate on channel 1 - just as an example
  // accelerometer code ---
  adxl.powerOn ();
  adxl.setRangeSetting (16); // Defina o intervalo, valores 2, 4, 8 ou 16
  // ---
}

void loop() {
  long start = millis();
  long total = cs_4_3.capacitiveSensor(30);
  bool pressedButton = !digitalRead(BUTTON);

  // accelerometer code ---
  int x, y, z;
  adxl.readAccel (& x, & y, & z);
  // ---

  // accelerometer code ---
  // x conditions
  if (x >= -5 && x <= 5){
    Serial.print(0);
    Serial.print(" ");
  }

  else {
    if (x < -5){  // front
      Serial.print(1);
      Serial.print(" ");
    }
    else if (x > 5){  // back
      Serial.print(2);
      Serial.print(" ");
    }
  } 

  // y conditions
  if (y >= -20 && y <= 20){
    Serial.print(0);
    Serial.print(" ");
  }

  else {
    if (y < -20){  // right
      Serial.print(1);
      Serial.print(" ");
    }
    else if (y > 20){  // left
      Serial.print(2);
      Serial.print(" ");
    }
  }

  Serial.print(z);
  Serial.print(" ");

  // normal values when not being touched "0, 4, 33" "x, y, z"


  if (total > 150) {
    touch = (touch + 1) % 4;  //modulo operator increments by 1 to iterate through 4 different planet views
    Serial.print(touch);
    Serial.print(" ");
  } else {
    Serial.print(touch);
    Serial.print(" ");
  }
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 200000);
  proximity = (duration * 0.034) / 2;  //Calculate the distance (in cm) based on the speed of sound.

  if (proximity < 30) {
    //interacted with sensor so turn on one light
    Serial.print(proximity);
    Serial.print(" ");
  } else {
    Serial.print(30);  //sends a maximum distance of 30 if its not below 30
    Serial.print(" ");
  }
  //can only press this when the planet was explored (when the 3 lights are turned on) or to start the app
  if (pressedButton) {
    buttonRandomNumber = random(6);  //maximum value of random numbers to 6 because of 6 planets
    Serial.println(buttonRandomNumber);
  } else {
    Serial.println(buttonRandomNumber);  //this will print the last state of the random number
  }
  delay(100);
}
