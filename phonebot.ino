//  by Brandon Plumbo AKA Riftliger
//  original version from Martyn Currey http://www.martyncurrey.com/
//  HC-05 universal test sketch to enter AT-commands
//  Uses hardware serial to talk to the host computer and software serial for communication with the bluetooth module
//
//  Pins
//  BT VCC to Arduino 5V out.
//  BT GND to GND
//  Arduino D11 to BT RX
//  Arduino D10 BT TX
//
//  When a command is entered in the serial monitor on the computer
//  the Arduino will relay it to the bluetooth module and display the result.
//
//  The HC-05 modules require both CR and NL

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); //  TX,RX

char c = ' ';
boolean NL = true;

void setup()
{

  Serial.begin(9600);
  Serial.println("Sketch HC-05");
  Serial.println("Arduino with HC-05 is ready");
  Serial.println("Make sure Both NL & CR are set");
  Serial.println("");

  //Set to HC-05 default baud rate, found using AT+UART.  It is usually 38400.
  BTSerial.begin(38400);
  Serial.println("BTserial started at 38400");
  Serial.println("");
  
}

void loop()
{

  // Read from the Bluetooth module and send to the Arduino Serial Monitor
  if (BTSerial.available())
  {
    c = BTSerial.read();
    Serial.write(c);
  }

  // Read from the Serial Monitor and send to the Bluetooth module
  if (Serial.available())
  {
    c = Serial.read();
    BTSerial.write(c);

    // Echo the user input to the main window. The ">" character indicates the user entered text.
    if (NL) {
      Serial.print(">");
      NL = false;
    }
    Serial.write(c);
    if (c == 10) {
      NL = true;
    }
  }
}

