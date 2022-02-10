#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
SoftwareSerial BTserial(10, 11);

bool useSerialMonitor = true;

int kickstand_servo = 0;
int left_servo = 1;
int right_servo = 2;

int volt_meter = A3;
int volt_meterValue = 0;

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
//    if(!useSerialMonitor){
//      pwm.wakeup();
//    }
  }
}

void sleep(){
  if(imAwake){
    imAwake = false;
    pwm.sleep();
  }
}

void rightKnob(int whatWhere){ 
  whatWhere = map(whatWhere, 0, 1024, 100, 500);
  if(useSerialMonitor){
    //Serial.println(String(whatWhere));//302 @high_end of off
    //400 @high_end_top_speed
  }
  pwm.setPWM(left_servo, 0, whatWhere); 
  //pwm.setPWM(right_servo, 0, whatWhere); 
}

void leftKnob(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, 100, 500);
  Serial.println(String(whatWhere));//284 @downpresser_end of off
  pwm.setPWM(right_servo, 0, whatWhere); 
  //pwm.setPWM(left_servo, 0, map(whatWhere, 500, 100, 100, 500)); 
}

void middleKickstand(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, 60, 280);
  Serial.println(String(whatWhere));//66 @full_morty
  pwm.setPWM(kickstand_servo, 0, whatWhere);
}

void checkVoltage(){
  volt_meterValue = analogRead(volt_meter);
  //Serial.println(String(volt_meterValue));//430 @usb_power
  /*
    430 @usb_power
   */
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
    /* 
      this sequence strategy 
      makes for a confusing switch
      */
    switch (aChar) {
         case '.':
          rightKnob(mess.toInt());
          mess = "";
          break;
             
         case 'l':
             //Serial.println(mess);
             mess = "";
             break;
             
         case 'm':
             leftKnob(mess.toInt());
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
    if(unAvailabits > 10){
      unAvailabits = 0;
      checkVoltage();
      sleep();
    }
  }

}
