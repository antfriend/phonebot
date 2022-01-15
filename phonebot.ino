#include <Arduino.h>
#include "HCPCA9685.h"
 
#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

int kickstand_servo = 0;
int left_servo = 1;
int right_servo = 2;

void park(){
  HCPCA9685.Servo(kickstand_servo, 90);
  HCPCA9685.Servo(left_servo, 90);
  HCPCA9685.Servo(right_servo, 90);
}

void forward(){
  HCPCA9685.Servo(left_servo, 200);
  HCPCA9685.Servo(right_servo, 200);
}

void backward(){
  HCPCA9685.Servo(left_servo, -10);
  HCPCA9685.Servo(right_servo, -10);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  park();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);  
  forward();
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  
  delay(3000);
  backward();
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
}
