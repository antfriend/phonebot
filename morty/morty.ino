#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
SoftwareSerial BTserial(10, 11);

bool useSerialMonitor = false;

int kickstand_servo = 0;
int left_servo = 1;
int right_servo = 2;

int  unAvailabits= 0;
bool isAvailable = false;
bool imAwake = false;

#define SERVOMIN  20 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


void wakeup(){
  if(!imAwake){
    imAwake = true;
    pwm.wakeup();
    //HCPCA9685.Sleep(false);
  }
}

void sleep(){
  if(imAwake){
    imAwake = false;
    pwm.sleep();
    //HCPCA9685.Sleep(true);    
  }
}

void leftWheel(int whatWhere){
//  if(whatWhere > 800){
////    for (uint16_t microsec = USMIN; microsec < USMAX; microsec = microsec + 10) {
////      pwm.writeMicroseconds(left_servo, microsec);
////    }
////    delay(1000);
//    //whatWhere = map(whatWhere, 800, 1024, 1500, 2000);
//    //Serial.println(String(whatWhere));
//    //pwm.writeMicroseconds(left_servo, 2150);
//    pwm.setPWM(left_servo, 0, 2150);
//    return;
//  }
  
  whatWhere = map(whatWhere, 0, 1024, 20, 500);
  if(useSerialMonitor){
    Serial.println(String(whatWhere));
  }
  pwm.setPWM(left_servo, 0, whatWhere);
  //HCPCA9685.Servo(left_servo, whatWhere);  
}

void rightWheel(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, 20, 500);
  //Serial.println(String(whatWhere));
  pwm.setPWM(right_servo, 0, whatWhere);
  //HCPCA9685.Servo(right_servo, whatWhere);  
}

void middleKickstand(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, -20, 350);
  //Serial.println(String(whatWhere));
  pwm.setPWM(kickstand_servo, 0, whatWhere);
  //HCPCA9685.Servo(kickstand_servo, whatWhere);  
}

void setup() {
  if(useSerialMonitor){
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Native USB only
    }
    Serial.println("begin");
  }


  //talk to Rick
  BTserial.begin(38400);

  //blinker
  pinMode(LED_BUILTIN, OUTPUT);

  //servos
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);
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
