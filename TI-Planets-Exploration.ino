#include <CapacitiveSensor.h>
// accelerometer code ---
#include <SPI.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>
// ---
#define BUTTON 2
#define BUTTON_help 5
#define echoPin 7
#define trigPin 8

#define LED_BUTTON 12
#define LED_CAPACITIVE 10
#define LED_PROX 9
#define LED_ACCEL 11

// order of the sent values: reset_aux, state, buttonRandomNumber , x, y, touch, proximity, flag_led_4

int touch = 0;

int x = 4;
int y = 5;
int z = 6;
long ellapsed;
float proximity;
long buttonRandomNumber = 0;
CapacitiveSensor cs_4_3 = CapacitiveSensor(4, 3);  // 10M resistor between pins 7 & 6, pin 6 is sensor pin, add a wire and or foil if desired
long duration;

// initiate the array for the displayed planets
int planets[] = {0, 1, 2, 3, 4, 5};
// initiate an auxiliar array to know when all the planets have been explored
int visited_planets[] = {-1, -1, -1, -1, -1, -1};
// planets visited count
int add_planet = 0;
int k = 0; // iteration inside the array

//int flag_button = 0;

int flag_led_1 = 0; // accel
int flag_led_2 = 0; // capacitive
int flag_led_3 = 0; // prox
int led_group = 0; // to count the number of LEDs that are "HIGH"
int flag_led_4 = 0; // button

int state = LOW;      // the current state of the output pin
//int reading;           // the current reading from the input pin
int previous = HIGH;    // the previous reading from the input pin

unsigned long time = 0;           // the last time the output pin was toggled
unsigned long debounce = 50UL;   // the debounce time, increase if the output flickers

// auxiliar variable to reset the planets to their original state (without the textures so the user knows that they weren't explored)
int reset_aux = 0;


// accelerometer code ---
ADXL345 adxl = ADXL345 ();
// ---
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_help, INPUT_PULLUP);
  pinMode(LED_BUTTON, OUTPUT);
  pinMode(LED_CAPACITIVE, OUTPUT);
  pinMode(LED_PROX, OUTPUT);
  pinMode(LED_ACCEL, OUTPUT);
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
  bool pressed_help_button = !digitalRead(BUTTON_help); // acelerometro

  // accelerometer code ---
  int x, y, z;
  adxl.readAccel (& x, & y, & z);

  if (pressedButton == LOW && state == LOW){
    if (reset_aux == 0){
          Serial.print(reset_aux);  // print the reset flag
          Serial.print(" ");
        }
  }

  if (pressedButton == HIGH && previous == LOW)
  {
    if (state == HIGH && led_group == 3){  // if in screen 1 and leds all light up

    // if in screen 1, verifies the LEDs to go back to screen 0
      flag_led_1 = 0;
      flag_led_2 = 0;
      flag_led_3 = 0;
      flag_led_4 = 0;
      digitalWrite(LED_CAPACITIVE, LOW);
      digitalWrite(LED_PROX, LOW);
      digitalWrite(LED_ACCEL, LOW);
      digitalWrite(LED_BUTTON, LOW);
      state = LOW;  // goes to screen 0
    }
    else if (state == LOW){   // if in screen 0
      
      if (add_planet == 6){  // if all planets have been explored
        reset_aux = 1;            // does the reset
        Serial.print(reset_aux);  // print the reset flag
        Serial.print(" ");
        state = LOW;
        for (int j = 0; j < 6; j++){  // fills the auxiliar array so it starts over
            if(visited_planets[j] != -1){
              visited_planets[j] = -1;
            }
          }
          reset_aux = 0;              
          add_planet = 0;
          k = 0;
      }
      else if (add_planet != 6){
        state = HIGH;               // goes to screen 1
        if (reset_aux == 0){
          Serial.print(reset_aux);  // print the reset flag
          Serial.print(" ");
        }
      }

      int random_int = random(6);  //maximum value of random numbers to 6 because of 6 planets
      while (k < 6){
        if (visited_planets[k] == -1){
          visited_planets[k] = planets[random_int];
          buttonRandomNumber = planets[random_int];
          add_planet += 1;
          k = 0;
          break;
        }
        else if (visited_planets[k] == planets[random_int]){  // if that planet was already explored, generate another random number
          //Serial.println("already exists.");
          //Serial.println("   ");
          random_int = random(6);
          k = -1;
        }
        k++;
      }
    }
  }

  previous = pressedButton;

  Serial.print(state); // toggle button state (if state = 0 -> screen 0 and if state = 1 -> screen 1)
  Serial.print(" ");
  Serial.print(buttonRandomNumber);  //this will print the last state of the random number
  Serial.print(" ");

  // accelerometer code ---
  if (state == HIGH){ // screen 1
    // x conditions
    if (x >= -5 && x < 10){  // LED
      Serial.print(0);
      Serial.print(" ");
    }
    else {
      if (x < -5){  // front
        flag_led_1 = 1;
        digitalWrite(LED_ACCEL, HIGH);
        Serial.print(-1);
        Serial.print(" ");
      }
      else if (x >= 10){  // back
        flag_led_1 = 1;
        digitalWrite(LED_ACCEL, HIGH);
        Serial.print(1);
        Serial.print(" ");
      }
    } 
  
    // y conditions
    if (y >= 30){
      Serial.print(0);
      Serial.print(" ");
    }
    else {
      if (y <= 0){  // right
        flag_led_1 = 1;
        digitalWrite(LED_ACCEL, HIGH);
        Serial.print(1);
        Serial.print(" ");
      }
      else if (y > 0 && y < 30){  // left
        flag_led_1 = 1;
        digitalWrite(LED_ACCEL, HIGH);
        Serial.print(-1);
        Serial.print(" ");
      }
    }

    if (total > 250) {  // if touch = 3 -> LED light up
      touch = (touch + 1) % 4;  //modulo operator increments by 1 to iterate through 4 different planet views
      if (touch == 3){
        flag_led_2 = 1;
        digitalWrite(LED_CAPACITIVE, HIGH);
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
      digitalWrite(LED_PROX, HIGH);
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
      digitalWrite(LED_BUTTON, HIGH);  // turn the LED on
      delay(300);                      
      digitalWrite(LED_BUTTON, LOW);   // turn the LED off
      delay(300); 
    }

    Serial.print(flag_led_4);
    Serial.println(" ");

   Serial.print(reset_aux);  // print the reset flag
    Serial.print(" ");

    delay(500);
  }

  else if (state == LOW){ // screen 0
    touch = 0;
    // accelerometer x
    Serial.print(0);
    Serial.print(" ");
    // accelerometer y
    Serial.print(0);
    Serial.print(" ");
    // touch
    Serial.print(touch);
    Serial.print(" ");
    // proximity
    Serial.print(30);  //sends a maximum distance of 30 if its not below 30
    Serial.print(" ");
    // led button
    Serial.print(flag_led_4);
    Serial.println(" ");
    delay(500);
  }
}
