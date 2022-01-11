#include <Arduino.h>
#include "ServoEasing.hpp"

ServoEasing Servo1;

#define START_DEGREE_VALUE 90

//Servo myservo; 
int myservo_pin = 5;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  if (Servo1.attach(myservo_pin, START_DEGREE_VALUE) == INVALID_SERVO) {
      //Serial.println(F("Error attaching servo"));
  }

}

void loop() {
  
  digitalWrite(LED_BUILTIN, HIGH);
  //Servo1.setSpeed(5);
  Servo1.easeTo(110);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
  //Servo1.setSpeed(20);
  Servo1.easeTo(70);
  delay(2000);
}
