#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
SoftwareSerial BTserial(10, 11);

bool useSerialMonitor = true;
int serialMonitorInc = 0;

int kickstand_servo = 0;
int left_servo = 1;
int right_servo = 2;
int left_servo_what = 0;
int right_servo_what = 0;

int volt_meter = A3;
int volt_meterValue = 0;

int mid_point_upper = 305;
int mid_point_lower = 285;

int  unAvailabits= 0;
bool isAvailable = false;
bool imAwake = false;
bool imUnconscious = false;

#define SERVOMIN  20 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

void wakeup(){
  if(!imAwake){
    imAwake = true;
    if(!imUnconscious){
      pwm.wakeup();
    }
  }
}

void sleep(){
  if(imAwake){
    imAwake = false;
    pwm.sleep();
  }
}

int MapKnob2ContinuousServo(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, 100, 500);
  //mid point map
  if(whatWhere < mid_point_upper){
    if(whatWhere > mid_point_lower){
      whatWhere = 0;
    }
  }
  return whatWhere;
}

int MapInvertedKnob2ContinuousServo(int whatWhere){
  whatWhere = map(whatWhere, 1024, 0, 100, 500);
  //mid point map
  if(whatWhere < mid_point_upper){
    if(whatWhere > mid_point_lower){
      whatWhere = 0;
    }
  }
  return whatWhere;
}

void leftKnob(int whatWhere){ 
  int invertedWhat = MapInvertedKnob2ContinuousServo(whatWhere);
  int normalWhat = MapKnob2ContinuousServo(whatWhere);
    //Serial.println(String(whatWhere));//302 @high_end of off
    //400 @high_end_top_speed
  if(normalWhat == 0 || invertedWhat ==0){
    left_servo_what = 0;
    right_servo_what = 0;
    return;
  }
  right_servo_what = normalWhat;
  left_servo_what = invertedWhat;
}

void rightKnob(int whatWhere){
  if(left_servo_what == 0){
    //left knob neutral
    whatWhere = MapKnob2ContinuousServo(whatWhere);
    left_servo_what = whatWhere;
    right_servo_what = whatWhere;
  }
//    whatWhere = MapKnob2ContinuousServo(whatWhere);
//    //Serial.println(String(whatWhere));//302 @high_end of off
//    //400 @high_end_top_speed
//  left_servo_what = whatWhere;
//  right_servo_what = map(whatWhere, 100, 500, 500, 100);
//  
//  if(left_servo_what == 0){
//    whatWhere = MapKnob2ContinuousServo(whatWhere);
//    if(whatWhere != 0){
//      left_servo_what = whatWhere;
//      right_servo_what = map(whatWhere, 100, 500, 500, 100);
      
//      if(whatWhere > 299){
//        whatWhere = whatWhere - 300;
//          if(left_servo_what > whatWhere){
//            //left_servo_what = left_servo_what - (whatWhere);
//          }
//          //right_servo_what = right_servo_what + whatWhere;
//          
//        }else{
//          whatWhere = whatWhere - 284;
//          //left_servo_what = left_servo_what + whatWhere;
//          if(right_servo_what > whatWhere){
//            //right_servo_what = right_servo_what - whatWhere;
//          }
//          
//        }
  

  //}
  
  if(useSerialMonitor){
    serialMonitorInc++;
    if(serialMonitorInc > 10){
      serialMonitorInc = 0;
      //Serial.print("\e[2J");
      Serial.println(" ");
      Serial.print(" left_servo_what ");
      Serial.println(String(left_servo_what));    
      Serial.print("       whatWhere ");
      Serial.println(String(whatWhere));//284 @downpresser_end of off
      Serial.print("right_servo_what ");
      Serial.println(String(right_servo_what));  
      Serial.print(" volt_meterValue ");
      Serial.println(String(volt_meterValue));//430 @usb_power  
      Serial.print("   imUnconscious ");
      Serial.println(String(imUnconscious));
      Serial.print(" kickstand_servo ");
      Serial.println(String(kickstand_servo));          
    }
  }
  
}

void middleKickstand(int whatWhere){
  whatWhere = map(whatWhere, 0, 1024, 60, 280);
  //Serial.println(String(whatWhere));//66 @full_morty
  pwm.setPWM(kickstand_servo, 0, whatWhere);
}

void checkVoltage(){
  /*
    430 @usb_power
    730 @lipo_power
   */
  volt_meterValue = analogRead(volt_meter);
  if(500 > volt_meterValue){
    //either usb(without lipo) or low power
    imUnconscious = true;
    sleep();
  }
}

void setup() {
  if(useSerialMonitor){
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Native USB only
    }
    Serial.println("begin");
    imUnconscious = false;
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
      asynchronous stream deserialization
      where the string looks like 
      "l###m###r###."
    */
    switch (aChar) {
         case '.':
          rightKnob(mess.toInt());
          mess = "";
          pwm.setPWM(right_servo, 0, right_servo_what); 
          pwm.setPWM(left_servo, 0, left_servo_what); 
          checkVoltage();
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
      sleep();
    }
  }

}
