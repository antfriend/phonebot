#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11);

int leftPotentiometer = A1;
int leftPotentiometerValue = 0;
int middlePotentiometer = A2;
int middlePotentiometerValue = 0;
int rightPotentiometer = A3;
int rightPotentiometerValue = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.begin(9600);
  BTserial.begin(38400);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Native USB only
//  }
//  Serial.println("Blarg!");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  leftPotentiometerValue = analogRead(leftPotentiometer);
  middlePotentiometerValue = analogRead(middlePotentiometer);
  rightPotentiometerValue = analogRead(rightPotentiometer);
  
  String mess = "l" + String(leftPotentiometerValue) + "m" + String(middlePotentiometerValue) + "r" + String(rightPotentiometerValue);
  BTserial.print(mess);//this works but no line ending
  BTserial.print(".");
  //delay(10);
  
  //BTserial.write(mess);//can't write a string
  //BTserial.println(mess);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
