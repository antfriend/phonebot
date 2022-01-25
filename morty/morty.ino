#include <Arduino.h>
#include "HCPCA9685.h"
#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11);

#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

int kickstand_servo = 0;
int left_servo = 1;
int right_servo = 2;

int  unAvailabits= 0;
bool isAvailable = false;
bool imAwake = false;

void wakeup(){
  if(!imAwake){
    imAwake = true;
    HCPCA9685.Sleep(false);
  }
}

void sleep(){
  if(imAwake){
    imAwake = false;
    HCPCA9685.Sleep(true);    
  }
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
  //Serial.println(String(whatWhere));
  HCPCA9685.Servo(kickstand_servo, whatWhere);  
}

void setup() {
  /* uncomment for serial debug, duh */
//  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Native USB only
//  }
//  Serial.println("begin");
  BTserial.begin(38400);

  pinMode(LED_BUILTIN, OUTPUT);
  HCPCA9685.Init(SERVO_MODE);
}

String mess = "";

void loop() {
  isAvailable = false;
  while (BTserial.available() > 0) {
    if(!isAvailable){
      digitalWrite(LED_BUILTIN, HIGH);
      isAvailable = true;
      unAvailabits= 0;
      wakeup();
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
  }else{
    unAvailabits++;
    if(unAvailabits > 100){
      unAvailabits = 0;
      sleep();
    }
  }

}
