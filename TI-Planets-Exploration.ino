/* MSc in Design and Multimedia - TI-Planets-Exploration
 *
 */
#include <CapacitiveSensor.h>
#define BUTTON 2
#define piezoBuzzer 5
#define echoPin 7  // Echo Pin
#define trigPin 8  // Trigger Pin
#define LEDPin 13

int touch = 0;
int x = 4;
int y = 5;
int z = 6;
long ellapsed;
float proximity; 
long buttonRandomNumber;
CapacitiveSensor cs_7_6 = CapacitiveSensor(7, 6);  // 10M resistor between pins 7 & 6, pin 6 is sensor pin, add a wire and or foil if desired
long duration;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);  // Use LED indicator (if required)
  pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(LEDPin, HIGH);
  randomSeed(100);
  cs_7_6.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turn off autocalibrate on channel 1 - just as an example
}

void loop() {
  long start = millis();
  long total = cs_7_6.capacitiveSensor(30);
  bool pressedButton = !digitalRead(BUTTON);

  //code for accelerometer
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.print(" ");

  if (total > 150) {
    touch = (touch + 1) % 3;  //modulo operator increments by 1 to iterate through 3 different planet shapes
    Serial.print(touch);
    Serial.print(" ");
  } else{
    Serial.print(touch);
    Serial.print(" ");
  }

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 200000);
  proximity = (duration * 0.034) / 2; //Calculate the distance (in cm) based on the speed of sound.

  //if obstacle at less 30cm distance
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
    buttonRandomNumber = random(7);  //maximum value of random numbers to 7 because of 7 planets
    Serial.println(buttonRandomNumber);
  } else {
      Serial.println(" ");
  }

  delay(100);
}