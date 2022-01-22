#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11);

int leftPotentiometer = A1;
int leftPotentiometerValue = 0;
int middlePotentiometer = A2;
int middlePotentiometerValue = 0;
int rightPotentiometer = A3;
int rightPotentiometerValue = 0;

void setup() {

  Serial.begin(9600);
  BTserial.begin(38400);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Native USB only
//  }
//  Serial.println("Blarg!");
}

void loop() {

  leftPotentiometerValue = analogRead(leftPotentiometer);
  //Serial.println(leftPotentiometerValue);
  BTserial.write(leftPotentiometerValue);
  delay(20);
  middlePotentiometerValue = analogRead(middlePotentiometer);
  delay(20);
  rightPotentiometerValue = analogRead(rightPotentiometer);
  delay(20);
  
}
