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
long buttonRandomNumber = 0;
CapacitiveSensor cs_4_3 = CapacitiveSensor(4, 3);  // 10M resistor between pins 7 & 6, pin 6 is sensor pin, add a wire and or foil if desired
long duration;

int planets[] = {0, 1, 2, 3, 4, 5};
int visited_planets[] = {-1, -1, -1, -1, -1, -1};
int add_planet = 0;
int k = 0; // iteration inside the array

//int flag_button = 0;

int flag_led_1 = 0;
int flag_led_2 = 0;
int flag_led_3 = 0;
int led_group = 0;
int flag_led_4 = 0;

int state = LOW;      // the current state of the output pin
//int reading;           // the current reading from the input pin
int previous = HIGH;    // the previous reading from the input pin

unsigned long time = 0;           // the last time the output pin was toggled
unsigned long debounce = 200UL;   // the debounce time, increase if the output flickers


// accelerometer code ---
ADXL345 adxl = ADXL345 ();
// ---
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  //randomSeed(100);
  randomSeed(analogRead(0));
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

  if (pressedButton == HIGH && previous == LOW && millis() - time > debounce) // add condition of (if LEDs light up != 3 then it's blocked)
  {
    if (state == HIGH && led_group == 3){

    // if in screen 1, verifies the LEDs to go back to screen 0
      flag_led_1 = 0;
      flag_led_2 = 0;
      flag_led_3 = 0;
      flag_led_4 = 0;
      state = LOW; 
    }
    else if (state == LOW){
      state = HIGH;
      //for (int l = 0; l < 6; l++){
      //Serial.println(visited_planets[l]);
      //}
      if (add_planet == 6){
          for (int j = 0; j < 6; j++){
            if(visited_planets[j] != -1){
              visited_planets[j] = -1;
            }
          }
          add_planet = 0;
          k = 0;
        }
      int random_int = random(6);  //maximum value of random numbers to 6 because of 6 planets
      //Serial.print("random: ");
      //Serial.print(random_int);
      //Serial.println(" ");
      while (k < 6){
        if (visited_planets[k] == -1){
          visited_planets[k] = planets[random_int];
          buttonRandomNumber = planets[random_int];
          add_planet += 1;
          k = 0;
          break;
        }
        else if (visited_planets[k] == planets[random_int]){
          //Serial.println("already exists.");
          //Serial.println("   ");
          random_int = random(6);
          k = -1;
        }
        k++;
      }
    }
    time = millis();
  }

  previous = pressedButton;

  Serial.print(state); // toggle button state (if state = 0 -> ecrã 0 and if state = 1 -> ecrã 1)
  Serial.print(" ");

  Serial.print(buttonRandomNumber);  //this will print the last state of the random number
  Serial.print(" ");

  // accelerometer code ---
  // x conditions
  if (x >= -5 && x <= 5){  // LED
    flag_led_1 = 1;
    Serial.print(0);
    Serial.print(" ");
  }

  else {
    if (x < -5){  // front
      Serial.print(-1);
      Serial.print(" ");
    }
    else if (x > 5){  // back
      Serial.print(1);
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
      Serial.print(-1);
      Serial.print(" ");
    }
  }

  //Serial.print(z);
  //Serial.print(" ");

  // normal values when not being touched "0, 4, 33" "x, y, z"

  //if (flag_led_1 == 1){
    // ligar o led
  //}
  //else if (flag_led_1 == 0){
    // desligar o led
  //}
  //if (flag_led_2 == 1){
    // ligar o led
  //}
  //else if (flag_led_2 == 0){
    // desligar o led
  //}
  //if (flag_led_3 == 1){
    // ligar o led
  //}
  //else if (flag_led_3 == 0){
    // desligar o led
  //}

  if (total > 250) {  // if touch = 3 -> LED light up
 
    touch = (touch + 1) % 4;  //modulo operator increments by 1 to iterate through 4 different planet views

    if (touch == 3){
      flag_led_2 = 1;
    }
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

  if (proximity < 30) {     // light one of the LEDs here
    flag_led_3 = 1;
    //interacted with sensor so turn on one light
    Serial.print(proximity);
    Serial.print(" ");
  } else {
    Serial.print(30);  //sends a maximum distance of 30 if its not below 30
    Serial.print(" ");
  }

  led_group = flag_led_1 + flag_led_2 + flag_led_3;
  

  if (led_group == 3){
    flag_led_4 = 1;  // starts blinking the button LED
  }

  Serial.print(flag_led_4);
  Serial.println(" ");

  delay(500);
}
