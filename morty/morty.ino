#include <Arduino.h>
#include "HCPCA9685.h"
#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11);

#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

int kickstand_servo = 0;
int left_servo = 1;
int right_servo = 2;

int state = 0;

void wakeup(){
  HCPCA9685.Sleep(false);
  HCPCA9685.Servo(kickstand_servo, 180);
  HCPCA9685.Servo(left_servo, 90);
  HCPCA9685.Servo(right_servo, 90);
}

void clockwise(int howmuch){
  //howmuch = map(howmuch, 0, 1024, 0, 200);
  //howmuch = constrain(howmuch, 0, 200);
  HCPCA9685.Servo(left_servo, howmuch);
  HCPCA9685.Servo(right_servo, howmuch);
}

void counterclockwise(){
  HCPCA9685.Servo(left_servo, 10);
  HCPCA9685.Servo(right_servo, 10);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Blarg!");
  BTserial.begin(38400);

  if (BTserial.available() > 0) { // Checks whether data is comming from the serial port
    state = BTserial.read(); // Reads the data from the serial port
  }
  
  pinMode(LED_BUILTIN, OUTPUT);
  HCPCA9685.Init(SERVO_MODE);
  
  wakeup();
}

void loop() {
  if (BTserial.available() > 0) { // Checks whether data is comming from the serial port
    state = BTserial.read(); // Reads the data from the serial port
    //Serial.println("state=");
    //Serial.println(state);
    Serial.println(state);
    clockwise(state);
  }else{
    delay(100);
  }
  delay(20); 
  
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(200);
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(200); 
  
//  delay(200);
//  digitalWrite(LED_BUILTIN, HIGH);
//  
//  delay(500);
//  counterclockwise();
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(500);
}
