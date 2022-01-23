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
bool isAvailable = false;

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
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Native USB only
//  }
  Serial.println("begin");
  BTserial.begin(38400);

  pinMode(LED_BUILTIN, OUTPUT);
  HCPCA9685.Init(SERVO_MODE);
  wakeup();
}

String mess = "";

void loop() {
  while (BTserial.available() > 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    isAvailable = true;
    char aChar = BTserial.read();
    if(aChar == '.'){
      Serial.println(". ");
    }
    mess = mess + aChar;
    //Serial.println("here");
    Serial.print(aChar);
    //Serial.println(BTserial.readString());
    //Serial.write(BTserial.read());//this works, but one character at a time
    
    //Serial.println(BTserial.read());
    
    //Serial.println(" ");
    //state = BTserial.parseInt();
    //Serial.println("state=");
    //Serial.println(state);
    //
    
    //clockwise(state);
  }
  
  if(isAvailable){
    digitalWrite(LED_BUILTIN, LOW);
    //Serial.println(" ");
    isAvailable = false;
  }
  
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
