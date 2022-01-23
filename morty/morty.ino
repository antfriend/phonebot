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

void leftWheel(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, 0, 440);
  //Serial.println(String(whatWhere));
  HCPCA9685.Servo(left_servo, whatWhere);  
}

void rightWheel(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, 0, 440);
  //Serial.println(String(whatWhere));
  HCPCA9685.Servo(right_servo, whatWhere);  
}

void middleKickstand(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, -20, 350);
  Serial.println(String(whatWhere));
  HCPCA9685.Servo(kickstand_servo, whatWhere);  
}

void clockwise(int howmuch){
  howmuch = map(howmuch, 0, 1024, 0, 440);
  Serial.println(String(howmuch));
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
  HCPCA9685.Sleep(false);
  //wakeup();
}

String mess = "";
String lefty = "";
String middlin = "";
String righty = "";

void loop() {
  while (BTserial.available() > 0) {
    if(!isAvailable){
      digitalWrite(LED_BUILTIN, HIGH);
      isAvailable = true;
    }
    char aChar = BTserial.read();
    
    switch (aChar) {
         case '.':
          leftWheel(mess.toInt());
          
          mess = "";
          break;
             
         case 'l':
             //Serial.println(mess);
             mess = "";
             break;
             
         case 'm':
             rightWheel(mess.toInt());
             mess = "";
             break;
             
         case 'r':
             middleKickstand(mess.toInt());
             mess = "";
             break;
             
         default:
             mess = mess + aChar;
      }
  }
  
  if(isAvailable){
    digitalWrite(LED_BUILTIN, LOW);
    isAvailable = false;
  }

}
